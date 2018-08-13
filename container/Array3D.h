//
// Created by Maximilian Denninger on 13.08.18.
//

#ifndef SDFGEN_ARRAY3D_H
#define SDFGEN_ARRAY3D_H

#include <vector>

template<typename dataType>
class Array3D{
public:
	using InternalStorage = std::vector<std::vector<std::vector<dataType> > >;

	InternalStorage& getData(){ return m_data; }

private:
	InternalStorage m_data;
};


#endif //SDFGEN_ARRAY3D_H
