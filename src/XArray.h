#if !defined(AFX_XARRAY_H__D03AF296_9FA8_499F_A47D_155DDACDA5CC__INCLUDED_)
#define AFX_XARRAY_H__D03AF296_9FA8_499F_A47D_155DDACDA5CC__INCLUDED_

template<class _mytype>
class XArray  
{
public:
	XArray()
	{
		m_data = NULL;
		m_size = 0;
		m_resize = 0x10;
	}

	virtual ~XArray()
	{
		if (m_data)
			delete[] m_data;
	}

	static unsigned int s_block_size;

	void push(_mytype item)
	{
		if (!m_data)
		{
			m_data =  new _mytype[m_resize];
			m_bufsize = m_resize;
		}

		if (m_size == m_bufsize)
		{
			_mytype* old = m_data;
			m_data = new _mytype[m_size + m_resize];
			memcpy(m_data, old, sizeof(_mytype) * m_size);
			delete[] old;
			m_bufsize += m_resize;
		}
		m_data[m_size] = item;
		++m_size;
	}

	_mytype get(unsigned int index)
	{
		return m_data[index];
	}

	void set(unsigned int index, _mytype item)
	{
		m_data[index] = item;
	}

	unsigned int size()
	{
		return m_size;
	}

	void remove(unsigned int index)
	{
		for (unsigned int k = index; k < m_size; ++k)
			m_data[k] = m_data[k + 1];
		--m_size;
	}
	
	void removeAll() { m_size = 0; }

	_mytype& operator[](unsigned int index)
	{
		return m_data[index];
	}

protected:
	_mytype* m_data;
	unsigned int m_size;
	unsigned int m_bufsize;
	unsigned int m_resize;
};

#endif
