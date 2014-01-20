/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#ifndef PUNN_H
#define PUNN_H 

#include <neuralnetworks/LayeredNeuralNetwork.h>
#include <string>
#include <vector>


namespace Neural {

	/** Type of a PUNN */
	enum PunnTopologyType { PUNN_TOPOLOGY_ALTERNATE, PUNN_TOPOLOGY_ALTERNATE_CONNECTED_ON_LAST_TWO_LAYERS, PUNN_TOPOLOGY_FIXED, PUNN_TOPOLOGY_FIXED_CONNECTED_ON_LAST_TWO_LAYERS };

	enum PunnProductNeuronNormalizationType { PUNN_PRODUCT_NEURON_NORMALIZATION_ABS, PUNN_PRODUCT_NEURON_NORMALIZATION_ADDED_CONSTANT, PUNN_PRODUCT_NEURON_NORMALIZATION_INTEGER_EXPOSANT };

	/**
	 * A Product Unit Neural Network
	 * @author Leo Cazenille <leo.cazenille@gmail.com>
	 */
	class PUNN : public LayeredNeuralNetwork {

		protected:

			/** Type of PUNN topology */
			PunnTopologyType _topologyType;

			/** Number of product neurons per layer */
			std::vector<unsigned int> _nbProductNeuronsPerLayer;

			/** Product Neurons Normalization Type */
			PunnProductNeuronNormalizationType _productNeuronsNormalization;

			/**
			 * Step when using PUNN_TOPOLOGY_ALTERNATE topology
			 */
			virtual void topologyAlternateStep();

			/**
			 * Step when using PUNN_TOPOLOGY_ALTERNATE_CONNECTED_ON_LAST_TWO_LAYERS topology
			 */
			virtual void topologyAlternateConnectedOnLastTwoLayersStep();

			/**
			 * Step when using PUNN_TOPOLOGY_FIXED topology
			 */
			virtual void topologyFixedStep();

			/**
			 * Step when using PUNN_TOPOLOGY_FIXED_CONNECTED_ON_LAST_TWO_LAYERS topology
			 */
			virtual void topologyFixedConnectedOnLastTwoLayersStep();

			/**
			 * Init product neurons
			 */
			virtual void initNbProductNeuronsPerLayer(std::vector<unsigned int> nbProductNeuronsPerHiddenLayer);

			/**
			 * {@InheritDoc}
			 */
			virtual unsigned int computeRequiredNumberOfWeights();


		public:

			// -+-+-  Constructors/Destructors  -+-+- //

			PUNN(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, PunnTopologyType topologyType = PUNN_TOPOLOGY_ALTERNATE, PunnProductNeuronNormalizationType productNeuronsNormalization = PUNN_PRODUCT_NEURON_NORMALIZATION_ABS, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			PUNN(unsigned int nbInputs, unsigned int nbOutputs, PunnTopologyType topologyType = PUNN_TOPOLOGY_ALTERNATE, PunnProductNeuronNormalizationType productNeuronsNormalization = PUNN_PRODUCT_NEURON_NORMALIZATION_ABS, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			PUNN(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, PunnTopologyType topologyType, std::vector<unsigned int>& nbNeuronsPerHiddenLayer, std::vector<unsigned int>& nbProductNeuronsPerLayer, PunnProductNeuronNormalizationType productNeuronsNormalization = PUNN_PRODUCT_NEURON_NORMALIZATION_ABS, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			PUNN(unsigned int nbInputs, unsigned int nbOutputs, PunnTopologyType topologyType, std::vector<unsigned int>& nbNeuronsPerHiddenLayer, std::vector<unsigned int>& nbProductNeuronsPerLayer, PunnProductNeuronNormalizationType productNeuronsNormalization = PUNN_PRODUCT_NEURON_NORMALIZATION_ABS, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			/** Deep Copy constructor */
			PUNN(PUNN const& other);
			virtual ~PUNN();


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual PUNN* clone() const;

			/**
			 * {@InheritDoc}
			 */
			virtual std::string toString() const;

			/**
			 * {@InheritDoc}
			 */
			virtual void step();

			/**
			 * Return a string identifying this class
			 */
			static std::string getNNTypeName();

	};

}


#endif


