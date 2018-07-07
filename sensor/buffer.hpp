#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

template <class T> 
class Buffer {

	public:

		Buffer (unsigned int tam);
		void insert (const T & x);
		T getPos (unsigned int position) const;
		T getFirst () const;
		T getLast () const;

	private:

		unsigned int i;
		unsigned int size;
		T *pT;
};

template <class T> 
Buffer<T>::Buffer (unsigned int tam){
  i = 0;
  size = tam;
  pT = new T[tam];
};

template <class T> 
void Buffer<T>::insert (const T & x){
  i = (i + 1) % size;
  pT[i] = x;
};

template <class T> 
T Buffer<T>::getPos (unsigned int position) const{
  return pT[(i + (size - position)) % size];
};

template <class T> 
T Buffer<T>::getLast () const{
  return pT[(i + size) % size];
};

template <class T> 
T Buffer<T>::getFirst () const{
  return pT[ 1 % size];
};

#endif /* __BUFFER_HPP__ */