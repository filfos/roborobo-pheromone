/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#include <neuralnetworks/LayeredNeuralNetwork.h>
#include <sstream>
#include <iostream>

using namespace Neural;


/* --------------------- LayeredNeuralNetwork --------------------- */

LayeredNeuralNetwork::~LayeredNeuralNetwork() {
	// ...
}

LayeredNeuralNetwork::LayeredNeuralNetwork(
		std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		NeuralNetwork(weights, nbInputs, nbOutputs), _activeBiais(activeBiais),
		_onlyUseBiaisForFirstHiddenLayer(onlyUseBiaisForFirstHiddenLayer),
		_biaisValue(biaisValue) {
	initNbNeuronsPerLayer(*new std::vector<unsigned int>(0));
}

LayeredNeuralNetwork::LayeredNeuralNetwork(
		unsigned int nbInputs,
		unsigned int nbOutputs,
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		NeuralNetwork(nbInputs, nbOutputs), _activeBiais(activeBiais),
		_onlyUseBiaisForFirstHiddenLayer(onlyUseBiaisForFirstHiddenLayer),
		_biaisValue(biaisValue) {
	initNbNeuronsPerLayer(*new std::vector<unsigned int>(0));
}

LayeredNeuralNetwork::LayeredNeuralNetwork(
		std::vector<double>& weights,
		unsigned int nbInputs,
		unsigned int nbOutputs,
		std::vector<unsigned int>& nbNeuronsPerHiddenLayer = *(new std::vector<unsigned int>(0)),
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		NeuralNetwork(weights, nbInputs, nbOutputs), _activeBiais(activeBiais),
		_onlyUseBiaisForFirstHiddenLayer(onlyUseBiaisForFirstHiddenLayer),
		_biaisValue(biaisValue) {
	initNbNeuronsPerLayer(nbNeuronsPerHiddenLayer);
}

LayeredNeuralNetwork::LayeredNeuralNetwork(
		unsigned int nbInputs,
		unsigned int nbOutputs,
		std::vector<unsigned int>& nbNeuronsPerHiddenLayer = *(new std::vector<unsigned int>(0)),
		bool activeBiais,
		bool onlyUseBiaisForFirstHiddenLayer,
		double biaisValue) :
		NeuralNetwork(nbInputs, nbOutputs), _activeBiais(activeBiais),
		_onlyUseBiaisForFirstHiddenLayer(onlyUseBiaisForFirstHiddenLayer),
		_biaisValue(biaisValue) {
	initNbNeuronsPerLayer(nbNeuronsPerHiddenLayer);
}


LayeredNeuralNetwork::LayeredNeuralNetwork(LayeredNeuralNetwork const& other) : NeuralNetwork(other) {
	_nbNeuronsPerLayer = other._nbNeuronsPerLayer;
	_activeBiais = other._activeBiais;
	_onlyUseBiaisForFirstHiddenLayer = other._onlyUseBiaisForFirstHiddenLayer;
	_biaisValue = other._biaisValue;
}

void LayeredNeuralNetwork::initNbNeuronsPerLayer(std::vector<unsigned int>& nbNeuronsPerHiddenLayer) {
	_nbNeuronsPerLayer = std::vector<unsigned int>(nbNeuronsPerHiddenLayer.size() + 2);
	_nbNeuronsPerLayer[0] = _nbInputs;
	for(size_t i = 0; i < nbNeuronsPerHiddenLayer.size(); i++)
		_nbNeuronsPerLayer[1 + i] = nbNeuronsPerHiddenLayer[i];
	_nbNeuronsPerLayer[nbNeuronsPerHiddenLayer.size() + 1] = _nbOutputs;
}

std::string LayeredNeuralNetwork::toString() const {
	return NeuralNetwork::toString();
}

bool LayeredNeuralNetwork::getActiveBiais() const {
	return _activeBiais;
}

bool LayeredNeuralNetwork::getOnlyUseBiaisForFirstHiddenLayer() const {
	return _onlyUseBiaisForFirstHiddenLayer;
}

double LayeredNeuralNetwork::getBiaisValue() const {
	return _biaisValue;
}


