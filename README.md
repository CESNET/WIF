# WIF - Weak Indication Framework
## Description
C++ library for fast development of (heterogeneous) detection and classification modules for (Encrypted) Network Traffic Analysis - (E)NTA. The library contains the most commonly used methods for ENTA. Therefore, WIF ims to minimize the time between the detection of a new threat and the deployment of tailored module for its detection.

WIF contains following structure and objects:
- Classifiers
	- Pattern-matching via Regex
	- IP blocklists
	- Machine Learning via scikit-learn
		- Possible interconnection with [ALF](https://github.com/CESNET/ALF)
- Combinators
	- Average
	- Dempster-Shafer Theory
	- Majority
	- Sum
- Reporters
	- Unirec
- Data storage classes
	- Classification result (*ClfResult*)
	- IP address
	- Network IP flow (*FlowFeatures*)
- Utils
	- IP prefix (range or subnet)
	- Timer

Classifiers perform traffic classification and threat detection. Combinators are used to fuse weak results together to obtain more robust and accurate result. Reporters are used for additional data exfiltration from modules for increased explainability. Utils are mainky used by other parts of the library but are made available for others as well. For more info, we would kindly refer to the section **Using WIF for Development**.

Note that Unirec Repoter and ALF Classifier are only available when `BUILD_WITH_UNIREC` is enabled, described in the section **Build & Installation** below.

## Requirements
- Python3.6 or Python3.8 devel with numpy:
	1. python36-devel, python36-numpy
	1. python38-devel, python38-numpy

Optionally, for additional features (`BUILD_WITH_UNIREC` option must be enabled manually):
- [CESNET/Nemea](https://github.com/CESNET/Nemea), mainly libtrap, libunirec, and libunirec++

## Build & Installation
### Build from Source
```
git clone https://github.com/CESNET/WIF.git
cd WIF
make
# For setting installation folders etc.
ccmake build
sudo make install
```

### Build and Install RPM
```
git clone https://github.com/CESNET/WIF.git
cd WIF
make rpm
sudo rpm -i <pathToRpmOutputtedByPreviousCommand>
```

## Documentation
Doxygen documentation can be generated by calling:
```
make docs
```

## Using WIF for Development

### Preparation
WIF-based module should firstly transform data into *FlowFeatures* object as it is used as input to classifiers. *FlowFeatures* is a wrapper over **std::vector** of allowed types, see *DataVariant* class. The recommended use is to receive network flows periodically in a loop and run it through the implemented classification method. Therefore, *FlowFeatures* should keep its layout: contain the same features on the same indexes throught the processing.
```
#include <wif/storage/flowFeatures.hpp>
WIF::FlowFeatures flow(NUMBER_OF_FLOW_ELEMENTS);
```

### Classification
All classifiers share a common interface define by an abstract *Classifier* class. The *classify()* takes either *FlowFeatures* or *std::vector\<WIF::FlowFeatures\>* and performs the classification. However, *setSourceFeatureIDs()* msut be called before the first *classify()* call. This method sets source indexes of the *FlowFeatures* which will be processed by the classifier.

The return type of *classify()* is *ClfResult* - variant holding either *double* (result of IP-blocklist-based detection: 0 or 1) or *std::vector\<double\>* (result of Machine Learning: array of probablities of each class). Each classifier defines what value type the *ClfResult* holds and what it means. The value can be obtained by one of the following calls:
```
clfResult.get<double>();
clfResult.get<std::vector<double>>();
```

The code example below shows how to correctly use *IpPrefixClassifier*:
```
#include <wif/classifiers/ipPrefixClassifier.hpp>

constexpr WIF::FeatureID SRC_IP_ID = 0;
constexpr WIF::FeatureID DST_IP_ID = 1;

...

std::vector<WIF::IpPrefix> blocklist = { WIF::IpPrefix("10.0.0.0/28") };
WIF::IpPrefixClassifier clf(blocklist);
clf.setSourceFeatureIDs({
	SRC_IP_ID,
	DST_IP_ID
});

...

while (record = receiveRecord()) {
	WIF::FlowFeatures flow(2);
	flow.set<WIF::IpAddress>(SRC_IP_ID, extractSrcIp(record));
	flow.set<WIF::IpAddress>(DST_IP_ID, extractDstIp(record));

	WIF::ClfResult res = clf.classify(flow);
	if (res.get<double>() > 0) {
		std::cout << "Blocklisted communication detected!" << std::endl;
	}
}
```

### Scikit-learn Interconnection
Python C API is used for performing ML-based classification in *ScikitMlClassifier* and *AlfClassifier*. Two files are required to perform this task. The first one is an actual ML model in a [pickle](https://docs.python.org/3/library/pickle.html) format. The second one is called *bridge*. This file must contain two functions:
1. `init(model_path)`
	- Obtains a string with path to the ML model, loads it and returns it
2. `classify(classifier, features)`
	- Obtains the loaded ML model and 2D array of features to classify, calls `predict_proba()`, and returns the output

Example `bridge.py` is shown below and can be used for many tasks:
```
import pickle


def init(model_path):
    with open(model_path, 'rb') as f:
        return pickle.load(f)


def classify(classifier, features):
    try:
        return classifier.predict_proba(features).tolist()
    except Exception as e:
        print(e)
        return []
```

### Combination
Combinators perform data combination and fusion. The interface of this object group is defined by abstract *Combinator* class. No prior method must be called before usage, all initialization is performed in constructors. Then, *combine()* method performs the actual combination.
```
#include <wif/combinators/averageCombinator.hpp>
constexpr double THRESHOLD_FOR_POSITIVE_DETECTION 0.65

WIF::AverageCombinator avgCom;
...

double averageScore = avgCom.combine({tlsSniScore, mlProba, blocklistScore});
if (averageScore >= THRESHOLD_FOR_POSITIVE_DETECTION) {
	std::cout << "Positive detection!" << std::endl;
}
```

### Reporters
The common interface of Reporters is defined by abstract *Reporter* object. Currently, the only available reporter is *UnirecRepoter* which is built on top of the [libunirec](https://github.com/CESNET/Nemea-Framework/tree/master/unirec). The function of reporters can be described as Finite State Automatons (FSM). Firstly, *onRecordStart()* must be called, to indicate a start of a new message. Then, *report(DataVariant)* can be called periodically. At the end, *onRecordEnd()* is called to indicate that the record can be sent to output. However, buffering may be used and record does not need to be sent right away. Use *flush()* to send out pending records.

In the case of *UnirecReporter*, *report(DataVariant)* method extracts the value held by the passed *DataVariant*, and it is used as a value of the next unirec field. Then, field ID is incremented and the next call of *report()* will set value to the next field. Therefore, it is required to follow the correct field order.

The example code for using *UnirecReporter* is shown below. For the documentation and usage of `Nemea`, we would kindly refer the reader to the [Nemea](https://github.com/CESNET/Nemea) and [libunirec](https://github.com/CESNET/Nemea-Framework/tree/master/unirec) repositories.
```
const std::string REPORTER_TEMPLATE = "double TLS_SNI_SCORE,uint8 DETECTION_RESULT";

Nemea::UnirecOutputInterface reporterIfc = unirec.buildOutputInterface();
reporterIfc.changeTemplate(REPORTER_TEMPLATE);
WIF::UnirecReporter unirecReporter(reporterIfc);

...

// Notice: the values are reported in the same order as defined in the REPORTER_TEMPLATE
unirecReporter.onRecordStart();
unirecReporter.report(tlsSniScore);
unirecReporter.report(detectionResult);
unirecReporter.onRecordEnd();
unirecReporter.flush();

...
```

## Contact
If you have any questions or problems, you are welcomed to send an email to [richard.plny@cesnet.cz](mailto:richard.plny@cesnet.cz).

## License
This project is distributed under the [BSD-3-Clause license](LICENSE).
<br>
&copy; 2024, CESNET z.s.p.o.
