#ifndef MY_VECTOR
#define MY_VECTOR

#include<mutex>
#include<cstring>

template <class T>
class  Vector {
public:
  Vector();
  Vector(unsigned int size);
  Vector(unsigned int size, const T & initial);
  Vector(const Vector<T>& v);
  Vector(Vector<T>&& v);
  Vector(std::initializer_list<T> list);
  ~Vector();

  Vector<T>& operator= (const Vector<T>& v);

  void push_back(const T& value);
  void pop_back();
  unsigned int size() const;
  T& at(unsigned int index);

  void _range_check(unsigned int index) const {
    if (index >= _size) {
      //should throw exception here.
      //__throw_out_of_range("vector::_M_range_check");
      std::cout << "vector::_M_range_check" << std::endl;
    }
  }

private:
  unsigned int _size;
  unsigned int _capacity;
  T* _buffer;
  std::mutex _mutex;
};

template<class T>
Vector<T>::Vector() {
  std::lock_guard<std::mutex> lck(_mutex);
  _capacity = 0;
  _size = 0;
  _buffer = 0;
}

template<class T>
Vector<T>::Vector(unsigned int size) {
  std::lock_guard<std::mutex> lck(_mutex);
  _size = size;
  unsigned int _bit = 0;
  while(size != 0) {
    size = size >> 1;
    ++_bit;
  }
  _capacity = 1 << _bit;
  _buffer = new T[_capacity];
}

template<class T>
Vector<T>::Vector(unsigned int size, const T & initial) {
  std::lock_guard<std::mutex> lck(_mutex);
  _size = size;
  unsigned int _bit = 0;
  while(size != 0) {
    size = size >> 1;
    ++_bit;
  }
  _capacity = 1 << _bit;
  _buffer = new T[_capacity];
  for (unsigned int i = 0; i < _size; i++)
    _buffer[i] = initial;
}

template<class T>
Vector<T>::Vector(const Vector<T> & v) {
  std::lock_guard<std::mutex> lck(_mutex);
  _size = v._size;
  _capacity = v._capacity;
  _buffer = new T[_capacity];
  for (unsigned int i = 0; i < _size; i++)
    _buffer[i] = v._buffer[i];
}

template<class T>
Vector<T>::Vector(Vector<T> && v) {
  std::lock_guard<std::mutex> lck(_mutex);
  _capacity = std::move(v._capacity);
  _size = std::move(v._size);
  _buffer =std::move(v._buffer);
}

template<class T>
Vector<T>::Vector(std::initializer_list<T> list) {
  std::lock_guard<std::mutex> lck(_mutex);
  unsigned int size = list.size();
  _size = size;
  unsigned int _bit = 0;
  while(size != 0) {
    size = size >> 1;
    ++_bit;
  }
  _capacity = 1 << _bit;
  _buffer = new T[_capacity];
  int i = 0;
  for(auto value : list) 
    _buffer[i++] = value;
}

template<class T>
Vector<T> & Vector<T>::operator = (const Vector<T> & v) {
  std::lock_guard<std::mutex> lck(_mutex);
  delete[] _buffer;
  _size = v._size;
  _capacity = v._capacity;
  _buffer = new T[_capacity];
  for (unsigned int i = 0; i < _size; i++)
    _buffer[i] = v._buffer[i];
  return *this;
}

template<class T>
Vector<T>::~Vector() {
  std::lock_guard<std::mutex> lck(_mutex);
  _size = 0;
  _capacity = 0;
  delete[] _buffer;
}

template<class T>
void Vector<T>::push_back(const T & v) {
  std::lock_guard<std::mutex> lck(_mutex);
  if (_size >= _capacity) {
    _capacity = (_capacity == 0 ? 1 : _capacity << 1);
    T* new_buffer = new T[_capacity];
    memcpy(new_buffer, _buffer, _size * sizeof(T));
    delete[] _buffer;
    _buffer = new_buffer;
  }
  _buffer[_size++] = v;
}

template<class T>
void Vector<T>::pop_back() {
  std::lock_guard<std::mutex> lck(_mutex);
  _size--;
}

template<class T>
unsigned int Vector<T>::size()const {
  return _size;
}

template<class T>
T& Vector<T>::at(unsigned int index) {
  _range_check(index);
  return _buffer[index];
}

#endif