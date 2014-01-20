/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#include <neuralnetworks/PUNN.h>
#include <neuralnetworks/NeuralNetwork.h>
#include <sstream>
#include <math.h>

using namespace Neural;


/* --------------------- PUNN --------------------- */

PUNN::PUNN(std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		PunnTopologyType topologyType,
		PunnProductNeuronNormalizationType productNeuronsNormalization,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		LayeredNeuralNetwork(weights, nbInputs, nbOutputs, activeBiais, onlyUseBiaisForFirstHiddenLayer, biaisValue),
		_topologyType(topologyType),
		_productNeuronsNormalization(productNeuronsNormalization) {
	initNbProductNeuronsPerLayer(std::vector<unsigned int>(0));
}

PUNN::PUNN(unsigned int nbInputs,
		unsigned int nbOutputs,
		PunnTopologyType topologyType,
		PunnProductNeuronNormalizationType productNeuronsNormalization,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		LayeredNeuralNetwork(nbInputs, nbOutputs, activeBiais, onlyUseBiaisForFirstHiddenLayer, biaisValue),
		_topologyType(topologyType),
		_productNeuronsNormalization(productNeuronsNormalization) {
	initNbProductNeuronsPerLayer(std::vector<unsigned int>(0));
}

PUNN::PUNN(std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		PunnTopologyType topologyType,
		std::vector<unsigned int>& nbNeuronsPerLayer,
		std::vector<unsigned int>& nbProductNeuronsPerLayer,
		PunnProductNeuronNormalizationType productNeuronsNormalization,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		LayeredNeuralNetwork(weights, nbInputs, nbOutputs, nbNeuronsPerLayer, activeBiais, onlyUseBiaisForFirstHiddenLayer, biaisValue),
		_topologyType(topologyType),
		_productNeuronsNormalization(productNeuronsNormalization) {
	initNbProductNeuronsPerLayer(nbProductNeuronsPerLayer);
}

PUNN::PUNN(unsigned int nbInputs,
		unsigned int nbOutputs,
		PunnTopologyType topologyType,
		std::vector<unsigned int>& nbNeuronsPerLayer,
		std::vector<unsigned int>& nbProductNeuronsPerLayer,
		PunnProductNeuronNormalizationType productNeuronsNormalization,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		LayeredNeuralNetwork(nbInputs, nbOutputs, nbNeuronsPerLayer, activeBiais, onlyUseBiaisForFirstHiddenLayer, biaisValue),
		_topologyType(topologyType),
		_productNeuronsNormalization(productNeuronsNormalization) {
	initNbProductNeuronsPerLayer(nbProductNeuronsPerLayer);
}


PUNN::PUNN(PUNN const& other) : LayeredNeuralNetwork(other),
		_topologyType(other._topologyType),
		_nbProductNeuronsPerLayer(other._nbProductNeuronsPerLayer),
		_productNeuronsNormalization(other._productNeuronsNormalization) {
	// ...
}

PUNN::~PUNN() {
	// ...
}


std::string PUNN::toString() const {
	return LayeredNeuralNetwork::toString();
}


void PUNN::topologyAlternateStep() {
	std::vector<double> prec = _inputs;
	unsigned int weightsIndice = 0;
	std::vector<double> tmp;

	for(unsigned int k = 0; k < _nbNeuronsPerLayer.size() - 1; k++) {
		unsigned int const nbOutputs = _nbNeuronsPerLayer[k + 1];
		bool isProductLayer = (k % 2 == 1); // If this is a layer where we do multiplication

		// Do Stuffs ! (base neural computation)
		if(isProductLayer) {
			tmp = std::vector<double>(nbOutputs, 1.0);
			for(unsigned int i = 0; i < prec.size(); i++) {
				for(unsigned int j = 0; j < nbOutputs; j++) {
					switch(_productNeuronsNormalization) {
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ADDED_CONSTANT:
							tmp[j] *= pow((prec[i] /*+ 10.0*/), _weights[weightsIndice++] / 350.0);
							break;

						case PUNN_PRODUCT_NEURON_NORMALIZATION_INTEGER_EXPOSANT:
							tmp[j] *= pow((prec[i] /*+ 10.0*/), floor(_weights[weightsIndice++] / 10.0));
							break;

						default:
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ABS:
//							std::cout << "DEBUG1: " << tmp[j]
//								<< " "
//								<< (tmp[j] * pow(fabs(prec[i]), _weights[weightsIndice++] / 350.0))
//								<< " "
//								<< prec[i]
//								<< " "
//								<< _weights[weightsIndice+1] / 350.0
//								<< std::endl;
							if(prec[i] != 0.0)
								tmp[j] *= pow(fabs(prec[i]), _weights[weightsIndice++] / 350.0);
					}
				}
			}
		} else {
			tmp = std::vector<double>(nbOutputs, 0.0);
			for(unsigned int i = 0; i < prec.size(); i++) {
				for(unsigned int j = 0; j < nbOutputs; j++) {
					tmp[j] += prec[i] * _weights[weightsIndice++];
				}
			}
		}

		// Do computation of the biais
		if(_activeBiais && (k==0 || !_onlyUseBiaisForFirstHiddenLayer))
			for(unsigned int j = 0; j < nbOutputs; j++)
				tmp[j] += tanh(_weights[weightsIndice++]) * _biaisValue;

		// XXX
		// Tanh activation function
		for(unsigned int i = 0; i < nbOutputs; i++)
			tmp[i] = tanh(tmp[i]);

		prec = tmp;
	}
	//std::cout << "DEBUG1: " << tmp[0] << std::endl;

	_outputs = tmp;
}


void PUNN::topologyAlternateConnectedOnLastTwoLayersStep() {
	std::vector<double> prec = _inputs;
	std::vector<double> prec2(0);
	unsigned int weightsIndice = 0;
	std::vector<double> tmp;

	for(unsigned int k = 0; k < _nbNeuronsPerLayer.size() - 1; k++) {
		unsigned int const nbOutputs = _nbNeuronsPerLayer[k + 1];
		bool isProductLayer = (k % 2 == 1); // If this is a layer where we do multiplication

		// Do Stuffs ! (base neural computation)
		if(isProductLayer) {
			tmp = std::vector<double>(nbOutputs, 1.0);
			for(unsigned int i = 0; i < prec.size(); i++) {
				for(unsigned int j = 0; j < nbOutputs; j++) {
					switch(_productNeuronsNormalization) {
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ADDED_CONSTANT:
							tmp[j] *= pow((prec[i] /*+ 10.0*/), _weights[weightsIndice++] / 350.0);
							break;

						case PUNN_PRODUCT_NEURON_NORMALIZATION_INTEGER_EXPOSANT:
							tmp[j] *= pow((prec[i] /*+ 10.0*/), floor(_weights[weightsIndice++]) / 10.0);
							break;

						default:
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ABS:
							tmp[j] *= pow(fabs(prec[i]), _weights[weightsIndice++] / 350.0);
					}
				}
			}
		} else {
			tmp = std::vector<double>(nbOutputs, 0.0);
			for(unsigned int i = 0; i < prec.size(); i++) {
				for(unsigned int j = 0; j < nbOutputs; j++) {
					tmp[j] += prec[i] * _weights[weightsIndice++];
				}
			}
		}

		// Do computation of the biais
		if(_activeBiais && (k==0 || !_onlyUseBiaisForFirstHiddenLayer))
			for(unsigned int j = 0; j < nbOutputs; j++)
				tmp[j] += tanh(_weights[weightsIndice++]) * _biaisValue;

		// XXX
		// Tanh activation function
		for(unsigned int i = 0; i < nbOutputs; i++)
			tmp[i] = tanh(tmp[i]);

		prec2 = prec;
		prec = tmp;
	}
	//std::cout << "DEBUG1: " << tmp[0] << std::endl;

	_outputs = tmp;
}


void PUNN::topologyFixedStep() {
	std::vector<double> prec = _inputs;
	unsigned int weightsIndice = 0;
	std::vector<double> tmp;

	for(unsigned int k = 0; k < _nbNeuronsPerLayer.size() - 1; k++) {
		unsigned int const nbAdditiveOutputs = _nbNeuronsPerLayer[k + 1];
		unsigned int const nbProductOutputs = _nbProductNeuronsPerLayer[k + 1];
		unsigned int const nbOutputs = nbAdditiveOutputs + nbProductOutputs;

		tmp = std::vector<double>(nbOutputs, 0.0);

		// Compute additive neurons
		for(unsigned int i = 0; i < prec.size(); i++) {
			for(unsigned int j = 0; j < nbAdditiveOutputs; j++) {
				tmp[j] += prec[i] * _weights[weightsIndice++];
			}
		}

		// Compute product neurons
		for(unsigned int j = 0; j < nbProductOutputs; j++) {
			tmp[nbAdditiveOutputs + j] = 1.0;
		}
		for(unsigned int i = 0; i < prec.size(); i++) {
			for(unsigned int j = 0; j < nbProductOutputs; j++) {
				if(prec[i] != 0.0) {
					switch(_productNeuronsNormalization) {
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ADDED_CONSTANT:
							tmp[nbAdditiveOutputs + j] *= pow((prec[i] /*+ 10.0*/), _weights[weightsIndice++] / 350.0);
							break;

						case PUNN_PRODUCT_NEURON_NORMALIZATION_INTEGER_EXPOSANT:
							tmp[nbAdditiveOutputs + j] *= pow((prec[i] /*+ 10.0*/), floor(_weights[weightsIndice++] / 10.0));
							break;

						default:
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ABS:
							tmp[nbAdditiveOutputs + j] *= pow(fabs(prec[i]), _weights[weightsIndice++] / 350.0);
					}
				}
			}
		}

		// Do Stuffs ! (base neural computation)
		// Do computation of the biais
		if(_activeBiais && (k==0 || !_onlyUseBiaisForFirstHiddenLayer))
			for(unsigned int j = 0; j < nbOutputs; j++)
				tmp[j] += tanh(_weights[weightsIndice++]) * _biaisValue;

		// XXX
		// Tanh activation function
		for(unsigned int i = 0; i < nbOutputs; i++) {
			tmp[i] = tanh(tmp[i]);
		}

		prec = tmp;
	}
	//std::cout << "DEBUG1: " << tmp[0] << std::endl;

	_outputs = tmp;
}


void PUNN::topologyFixedConnectedOnLastTwoLayersStep() {
	std::vector<double> prec = _inputs;
	std::vector<double> prec2(0);
	unsigned int weightsIndice = 0;
	std::vector<double> tmp;

	for(unsigned int k = 0; k < _nbNeuronsPerLayer.size() - 1; k++) {
		unsigned int const nbAdditiveOutputs = _nbNeuronsPerLayer[k + 1];
		unsigned int const nbProductOutputs = _nbProductNeuronsPerLayer[k + 1];
		unsigned int const nbOutputs = nbAdditiveOutputs + nbProductOutputs;

		tmp = std::vector<double>(nbOutputs, 0.0);

		// Compute additive neurons
		for(unsigned int i = 0; i < prec.size(); i++) {
			for(unsigned int j = 0; j < nbAdditiveOutputs; j++) {
				tmp[j] += prec[i] * _weights[weightsIndice++];
			}
		}
		for(unsigned int i = 0; i < prec2.size(); i++) {
			for(unsigned int j = 0; j < nbAdditiveOutputs; j++) {
				tmp[j] += prec2[i] * _weights[weightsIndice++];
			}
		}

		// Compute product neurons
		for(unsigned int j = 0; j < nbProductOutputs; j++) {
			tmp[nbAdditiveOutputs + j] = 1.0;
		}
		for(unsigned int i = 0; i < prec.size(); i++) {
			for(unsigned int j = 0; j < nbProductOutputs; j++) {
				if(prec[i] != 0.0) {
					switch(_productNeuronsNormalization) {
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ADDED_CONSTANT:
							tmp[nbAdditiveOutputs + j] *= pow((prec[i] /*+ 10.0*/), _weights[weightsIndice++] / 350.0);
							break;

						case PUNN_PRODUCT_NEURON_NORMALIZATION_INTEGER_EXPOSANT:
							tmp[nbAdditiveOutputs + j] *= pow((prec[i] /*+ 10.0*/), floor(_weights[weightsIndice++] / 10.0));
							break;

						default:
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ABS:
							tmp[nbAdditiveOutputs + j] *= pow(fabs(prec[i]), _weights[weightsIndice++] / 350.0);
					}
				}
			}
		}
		for(unsigned int i = 0; i < prec2.size(); i++) {
			for(unsigned int j = 0; j < nbProductOutputs; j++) {
				if(prec2[i] != 0.0) {
					switch(_productNeuronsNormalization) {
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ADDED_CONSTANT:
							tmp[nbAdditiveOutputs + j] *= pow((prec2[i] /*+ 10.0*/), _weights[weightsIndice++] / 350.0);
							break;

						case PUNN_PRODUCT_NEURON_NORMALIZATION_INTEGER_EXPOSANT:
							tmp[nbAdditiveOutputs + j] *= pow((prec2[i] /*+ 10.0*/), floor(_weights[weightsIndice++] / 10.0));
							break;

						default:
						case PUNN_PRODUCT_NEURON_NORMALIZATION_ABS:
							tmp[nbAdditiveOutputs + j] *= pow(fabs(prec2[i]), _weights[weightsIndice++] / 350.0);
					}
				}
			}
		}

		// Do Stuffs ! (base neural computation)
		// Do computation of the biais
		if(_activeBiais && (k==0 || !_onlyUseBiaisForFirstHiddenLayer))
			for(unsigned int j = 0; j < nbOutputs; j++)
				tmp[j] += tanh(_weights[weightsIndice++]) * _biaisValue;

		// XXX
		// Tanh activation function
		for(unsigned int i = 0; i < nbOutputs; i++) {
			tmp[i] = tanh(tmp[i]);
		}

		prec2 = prec;
		prec = tmp;
	}
	//std::cout << "DEBUG1: " << tmp[0] << std::endl;

	_outputs = tmp;
}

void PUNN::step() {
	switch(_topologyType) {
		case PUNN_TOPOLOGY_FIXED:
			topologyFixedStep();
			break;

		case PUNN_TOPOLOGY_FIXED_CONNECTED_ON_LAST_TWO_LAYERS:
			topologyFixedConnectedOnLastTwoLayersStep();
			break;

		case PUNN_TOPOLOGY_ALTERNATE_CONNECTED_ON_LAST_TWO_LAYERS:
			topologyAlternateConnectedOnLastTwoLayersStep();
			break;

		default:
		case PUNN_TOPOLOGY_ALTERNATE:
			topologyAlternateStep();
	}
}

void PUNN::initNbProductNeuronsPerLayer(std::vector<unsigned int> nbProductNeuronsPerHiddenLayer) {
	_nbProductNeuronsPerLayer = std::vector<unsigned int>(nbProductNeuronsPerHiddenLayer.size() + 2);
	_nbProductNeuronsPerLayer[0] = 0;
	for(size_t i = 0; i < nbProductNeuronsPerHiddenLayer.size(); i++)
		_nbProductNeuronsPerLayer[1 + i] = nbProductNeuronsPerHiddenLayer[i];
	_nbProductNeuronsPerLayer[nbProductNeuronsPerHiddenLayer.size() + 1] = 0;
}


unsigned int PUNN::computeRequiredNumberOfWeights() {
	unsigned int res = 0;
	unsigned int nbBiais = 0;
	if(_activeBiais)
		nbBiais = 1;

	if(_nbNeuronsPerLayer.size() <= 2) {
		return (_nbInputs + nbBiais) * _nbOutputs;
	} else {

		switch(_topologyType) {
			case PUNN_TOPOLOGY_FIXED:
				res += (_nbInputs + nbBiais) * (_nbNeuronsPerLayer[1] + _nbProductNeuronsPerLayer[1]);
				if(_onlyUseBiaisForFirstHiddenLayer)
					nbBiais = 0;
				for(size_t i = 1; i < _nbNeuronsPerLayer.size() - 1; i++) {
					res += (_nbNeuronsPerLayer[i] + _nbProductNeuronsPerLayer[i] + nbBiais) *
						(_nbNeuronsPerLayer[i + 1] + _nbProductNeuronsPerLayer[i + 1]);
				}
				break;

			case PUNN_TOPOLOGY_FIXED_CONNECTED_ON_LAST_TWO_LAYERS:
				res += (_nbInputs + nbBiais) * (_nbNeuronsPerLayer[1] + _nbProductNeuronsPerLayer[1]);
				if(_onlyUseBiaisForFirstHiddenLayer)
					nbBiais = 0;
				for(size_t i = 1; i < _nbNeuronsPerLayer.size() - 1; i++) {
					res += (_nbNeuronsPerLayer[i] + _nbProductNeuronsPerLayer[i] + nbBiais) *
						(_nbNeuronsPerLayer[i + 1] + _nbProductNeuronsPerLayer[i + 1]);
					if(i < _nbNeuronsPerLayer.size() - 2)
						res += (_nbNeuronsPerLayer[i] + _nbProductNeuronsPerLayer[i] + nbBiais) *
							(_nbNeuronsPerLayer[i + 2] + _nbProductNeuronsPerLayer[i + 2]);
				}
				break;

			case PUNN_TOPOLOGY_ALTERNATE_CONNECTED_ON_LAST_TWO_LAYERS:
				res += (_nbInputs + nbBiais) * _nbNeuronsPerLayer[1];
				if(_onlyUseBiaisForFirstHiddenLayer)
					nbBiais = 0;
				for(size_t i = 1; i < _nbNeuronsPerLayer.size() - 1; i++) {
					bool isProductLayer = ((i - 1) % 2 == 1); // If this is a layer where we do multiplication
					if(isProductLayer) {
						res += (_nbNeuronsPerLayer[i] + nbBiais) * _nbNeuronsPerLayer[i + 1];
						if(i < _nbNeuronsPerLayer.size() - 2)
							res += (_nbNeuronsPerLayer[i] + nbBiais) * _nbNeuronsPerLayer[i + 2];
					} else {
						res += (_nbNeuronsPerLayer[i] + nbBiais) * _nbNeuronsPerLayer[i + 1];
						if(i < _nbNeuronsPerLayer.size() - 2)
							res += (_nbNeuronsPerLayer[i] + nbBiais) * _nbNeuronsPerLayer[i + 2];
					}
				}
				break;

			default:
			case PUNN_TOPOLOGY_ALTERNATE:
				res += (_nbInputs + nbBiais) * _nbNeuronsPerLayer[1];
				if(_onlyUseBiaisForFirstHiddenLayer)
					nbBiais = 0;
				for(size_t i = 1; i < _nbNeuronsPerLayer.size() - 1; i++) {
					bool isProductLayer = ((i - 1) % 2 == 1); // If this is a layer where we do multiplication
					if(isProductLayer) {
						res += (_nbNeuronsPerLayer[i] + nbBiais) * _nbNeuronsPerLayer[i + 1];
					} else {
						res += (_nbNeuronsPerLayer[i] + nbBiais) * _nbNeuronsPerLayer[i + 1];
					}
				}
		}

		return res;
	}
}


std::string PUNN::getNNTypeName() {
	return "PUNN";
}

PUNN* PUNN::clone() const {
	return new PUNN(*this);
}

