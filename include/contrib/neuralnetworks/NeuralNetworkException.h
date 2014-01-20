/**
 * @file
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 *
 *
 */

#ifndef NEURALNETWORKEXCEPTION_H
#define NEURALNETWORKEXCEPTION_H 

#include <stdexcept>
#include <exception>
#include <string>

namespace Neural {

	/**
	 * Exception class thrown what something bad happens
	 * @author Leo Cazenille <leo.cazenille@gmail.com>
	 */
	class NeuralNetworkException : public std::runtime_error {
		public :
			NeuralNetworkException(const std::string& what_arg);
	};

}

#endif

