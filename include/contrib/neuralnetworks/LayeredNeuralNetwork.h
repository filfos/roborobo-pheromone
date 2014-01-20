
/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#ifndef LAYEREDNEURALNETWORK_H
#define LAYEREDNEURALNETWORK_H 

#include <neuralnetworks/NeuralNetwork.h>

namespace Neural {

	/**
	 * Definition of a layered neural network
	 * @author Leo Cazenille <leo.cazenille@gmail.com>
	 */
	class LayeredNeuralNetwork : public NeuralNetwork {

		private:

			/**
			 * Initialize nbNeuronsPerLayer
			 */
			void initNbNeuronsPerLayer(std::vector<unsigned int>& nbNeuronsPerHiddenLayer);

		protected:

			/** Number of neurons per layer */
			std::vector<unsigned int> _nbNeuronsPerLayer;

			/** Says if we want biais neurons */
			bool _activeBiais;

			/** Says if we want the biais neurons to be apply to all hidden layers or just to the
			 *	first one.
			 */
			bool _onlyUseBiaisForFirstHiddenLayer;

			/** Values of biais neurons (typically 1) */
			double _biaisValue;


		public :

			// -+-+-  Constructors/Destructors  -+-+- //

			virtual ~LayeredNeuralNetwork();
			LayeredNeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			LayeredNeuralNetwork(unsigned int nbInputs, unsigned int nbOutputs, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			LayeredNeuralNetwork(std::vector<double>& weights, unsigned int nbInputs, unsigned int nbOutputs, std::vector<unsigned int>& nbNeuronsPerLayer, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			LayeredNeuralNetwork(unsigned int nbInputs, unsigned int nbOutputs, std::vector<unsigned int>& nbNeuronsPerLayer, bool activeBiais = false, bool onlyUseBiaisForFirstHiddenLayer = false, double biaisValue = 1.0);
			/** Deep Copy constructor */
			LayeredNeuralNetwork(LayeredNeuralNetwork const& other);


			// -+-+-  Accessors/Mutators  -+-+- //

			/**
			 * Accessor for activeBiais
			 */
			bool getActiveBiais() const;

			/**
			* Accessor for onlyUseBiaisForFirstHiddenLayer
			*/
			bool getOnlyUseBiaisForFirstHiddenLayer() const;

			/**
			* Accessor for biaisValue
			*/
			double getBiaisValue() const;


			// -+-+-  Main Methods  -+-+- //

			/**
			 * {@InheritDoc}
			 */
			virtual LayeredNeuralNetwork* clone() const = 0;

			/**
			 * {@InheritDoc}
			 */
			virtual std::string toString() const;

	};



}

#endif

