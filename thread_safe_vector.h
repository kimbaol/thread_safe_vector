#ifndef MY_VECTOR
#define MY_VECTOR

#include<mutex>
#include<cstring>

using size_type = unsigned int;

template <class T>
class  Vector {
public:
  Vector();
  Vector(size_type  size);
  Vector(size_type  size, const T & initial);
  Vector(const Vector<T>& v);
  Vector(Vector<T>&& v);
  Vector(std::initializer_list<T> list);
  ~Vector();

  Vector<T>& operator= (const Vector<T>& v);

  void push_back(const T& value);
  void pop_back();
  size_type  size() const;
  T& at(size_type  index);

private:
  size_type  m_size;
  size_type  m_capacity;
  T* m_buffer;
  std::mutex m_mutex;
};

template<class T>
Vector<T>::Vector() {
  std::lock_guard<std::mutex> lck(m_mutex);
  m_capacity = 0;
  m_size = 0;
  m_buffer = 0;
}

template<class T>
Vector<T>::Vector(size_type  size) {
  std::lock_guard<std::mutex> lck(m_mutex);
  m_size = size;
  size_type  _bit = 0;
  while(size != 0) {
    size = size >> 1;
    ++_bit;
  }
  m_capacity = 1 << _bit;
  m_buffer = new T[m_capacity];
}

template<class T>
Vector<T>::Vector(size_type  size, const T & initial) {
  std::lock_guard<std::mutex> lck(m_mutex);
  m_size = size;
  size_type  _bit = 0;
  while(size != 0) {
    size = size >> 1;
    ++_bit;
  }
  m_capacity = 1 << _bit;
  m_buffer = new T[m_capacity];
  for (size_type  i = 0; i < m_size; i++)
    m_buffer[i] = initial;
}

template<class T>
Vector<T>::Vector(const Vector<T> & v) {
  std::lock_guard<std::mutex> lck(m_mutex);
  m_size = v.m_size;
  m_capacity = v.m_capacity;
  m_buffer = new T[m_capacity];
  for (size_type  i = 0; i < m_size; i++)
    m_buffer[i] = v.m_buffer[i];
}

template<class T>
Vector<T>::Vector(Vector<T> && v) {
  std::lock_guard<std::mutex> lck(m_mutex);
  m_capacity = std::move(v.m_capacity);
  m_size = std::move(v.m_size);
  m_buffer =std::move(v.m_buffer);
}

template<class T>
Vector<T>::Vector(std::initializer_list<T> list) {
  std::lock_guard<std::mutex> lck(m_mutex);
  size_type  size = list.size();
  m_size = size;
  size_type  _bit = 0;
  while(size != 0) {
    size = size >> 1;
    ++_bit;
  }
  m_capacity = 1 << _bit;
  m_buffer = new T[m_capacity];
  int i = 0;
  for(auto &value : list) 
    m_buffer[i++] = value;
}

template<class T>
Vector<T> & Vector<T>::operator = (const Vector<T> & v) {
  std::lock_guard<std::mutex> lck(m_mutex);
  delete[] m_buffer;
  m_size = v.m_size;
  m_capacity = v.m_capacity;
  m_buffer = new T[m_capacity];
  for (size_type  i = 0; i < m_size; i++)
    m_buffer[i] = v.m_buffer[i];
  return *this;
}

template<class T>
Vector<T>::~Vector() {
  std::lock_guard<std::mutex> lck(m_mutex);
  m_size = 0;
  m_capacity = 0;
  delete[] m_buffer;
}

template<class T>
void Vector<T>::push_back(const T & v) {
  std::lock_guard<std::mutex> lck(m_mutex);
  if (m_size >= m_capacity) {
    m_capacity = (m_capacity == 0 ? 1 : m_capacity << 1);
    T* new_buffer = new T[m_capacity];
    memcpy(new_buffer, m_buffer, m_size * sizeof(T));
    delete[] m_buffer;
    m_buffer = new_buffer;
  }
  m_buffer[m_size++] = v;
}

template<class T>
void Vector<T>::pop_back() {
  std::lock_guard<std::mutex> lck(m_mutex);
  if (m_size == 0)
    throw std::out_of_range("pop_back from a empty vector!");
  --m_size;
  m_buffer[m_size].~T();
}

template<class T>
size_type  Vector<T>::size()const {
  return m_size;
}

template<class T>
T& Vector<T>::at(size_type  index) {
  if (index >= m_size) 
    throw std::out_of_range("index out of range!");
  return m_buffer[index];
}

#endif