# 网络层buffer
这个网络buffer代码还是比较好用的，内部使用vector存取数据，vector可以确保数据是连续的，所以可以大胆的使用数据指针去进行网络取值

## 使用方法
### 1.简单赋值
``` cpp
#include "messagebuffer.h"
#include <iostream>

using namespace std;

int main(void)
{
    try
    {
        MessageBuffer buf;
        uint8 cmd(0X01);
        string content("ABC");
        buf << cmd << content;
        uint8 cmd2(0),cmd3(0);
        buf > cmd2;     // cmd2 = 0x01;
        buf >> cmd3;    // cmd3 = 0x01;
        buf > cmd2;     // cmd2 = (ASCII of 'A')
        string buf2,buf3;
        buf2 = buf.Get(2);  // buf2 = "AB";
        buf3 = buf.Read(1); // buf3 = "A";
        buf2 = bug.Get(2);  // buf2 = "BC";
        buf3 = buf.Read(2); // buf3 = "BC";
        // now buf is empty!
    }
    catch(const BufferException& e)
    {
        cout << e._ret << e._msg << endl;
    }
}

```
### 2.与网络接口共用
``` cpp
#include "messagebuffer.h"
#include <iostream>

using namespace std;

int main(void)
{
    try
    {
        //忽略套接字建立与其他处理过程
        //接收
        MessageBuffer buf;
        int size(0);
        while ((size = ::read(fd, buf.GetWritePointer(), buf.GetRemainingSpace())) > 0)
        {
            buf.WriteCompleted(size);
            buf.EnsureFreeSpace();
        }
        //发送
        while ((size = ::write(fd, buf.GetReadPointer(), buf.GetActiveSize())) > 0)
        {
            buf.ReadCompleted(size);
        }
        buf.Normalize();
    }
    catch(const BufferException& e)
    {
        cout << e._ret << e._msg << endl;
    }
}

```

## 源码及注释
``` cpp
typedef unsigned char uint8;

#include <vector>
#include <cstring>
#include <string>

using std::string;

class BufferException
{
public:
	uint8_t _ret;
	string _msg;
public:
	BufferException(uint8_t ret, string msg)
		:_ret(ret)
		, _msg(msg)
	{}
};

class MessageBuffer
{
	typedef std::vector<uint8>::size_type size_type;

public:
	//空间4096默认构造
	MessageBuffer() : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(4096);
	}
	//空间大小构造
	explicit MessageBuffer(std::size_t initialSize) : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(initialSize);
	}
	//复制构造
	MessageBuffer(MessageBuffer const& right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right._storage)
	{
	}
	//右值复制构造
	MessageBuffer(MessageBuffer&& right) : _wpos(right._wpos), _rpos(right._rpos), _storage(right.Move()) { }

	void Reset()
	{
		_wpos = 0;
		_rpos = 0;
	}
	//改变空间大小
	void Resize(size_type bytes)
	{
		_storage.resize(bytes);
	}
	// 获得列表首元素地址
	uint8* GetBasePointer() { return _storage.data(); }
	// 获取未读数据首地址
	uint8* GetReadPointer() { return GetBasePointer() + _rpos; }
	// 获取已写数据末地址
	uint8* GetWritePointer() { return GetBasePointer() + _wpos; }
	// 已读空间增加
	void ReadCompleted(size_type bytes) { _rpos += bytes; }
	// 已写地址增加
	void WriteCompleted(size_type bytes) { _wpos += bytes; }
	// 过去尚未读取的空间数量
	size_type GetActiveSize() const { return _wpos - _rpos; }
	// 获取剩余空间
	size_type GetRemainingSpace() const { return _storage.size() - _wpos; }
	// 获取总空间大小
	size_type GetBufferSize() const { return _storage.size(); }

	// Discards inactive data
	// 将未读部分移动至空间起始地址，重置读写位
	void Normalize()
	{
		if (_rpos)
		{
			if (_rpos != _wpos)
				memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());
			_wpos -= _rpos;
			_rpos = 0;
		}
	}

	// 如果空间不足，则扩展50%的空间
	void EnsureFreeSpace()
	{
		// resize buffer if it's already full
		if (GetRemainingSpace() == 0)
			_storage.resize(_storage.size() * 3 / 2);
	}
	//写到当前所写的位置上面
	void Write(void const* data, std::size_t size)
	{
		if (size)
		{
			while (size >= GetRemainingSpace())
			{
				_storage.resize(_storage.size() * 3 / 2);
			}
			memcpy(GetWritePointer(), data, size);
			WriteCompleted(size);
		}
	}
	//写到当前所写的位置上面
	void Write(const string& str)
	{
		Write(str.data(), str.size());
	}

	MessageBuffer& operator<<(MessageBuffer& val)
	{
		Write(val.GetReadPointer(), val.GetActiveSize());
		return *this;
	}

	MessageBuffer& operator<<(const string& val)
	{
		Write(val);
		return *this;
	}

	template <typename T>
	MessageBuffer& operator<<(const T& val)
	{
		Write(&val, sizeof(decltype(val)));
		return *this;
	}

	template <typename T>
	MessageBuffer& operator >> (T& val)
	{
		this->Read((void*)&val, sizeof(decltype(val)));
		return *this;
	}

	template <typename T>
	MessageBuffer& operator > (T& val)
	{
		this->Get((void*)&val, sizeof(decltype(val)));
		return *this;
	}

	void Read(void* data, const size_type& size)
	{
		if (size > GetActiveSize())throw BufferException(1, "out of");
		memcpy(data, GetReadPointer(), size);
		ReadCompleted(size);
	}

	void Get(void* data, const size_type& size)
	{
		if (size > GetActiveSize())throw BufferException(1, "out of");
		memcpy(data, GetReadPointer(), size);
	}
	string Read(const size_type& size)
	{
		if (size > GetActiveSize())throw BufferException(1, "out of");
		string str;
		str.assign((const char*)GetReadPointer(), size);
		ReadCompleted(size);
		return move(str);
	}

	string Get(const size_type& size)
	{
		if (size > GetActiveSize())throw BufferException(1, "out of");
		string str;
		str.assign((const char*)GetReadPointer(), size);
		return move(str);
	}

	string tostring()
	{
		return string((char *)this->GetReadPointer(), this->GetActiveSize());
	}

	//取右值
	std::vector<uint8>&& Move()
	{
		_wpos = 0;
		_rpos = 0;
		return std::move(_storage);
	}

	MessageBuffer& operator=(MessageBuffer const& right)
	{
		if (this != &right)
		{
			_wpos = right._wpos;
			_rpos = right._rpos;
			_storage = right._storage;
		}

		return *this;
	}

	MessageBuffer& operator=(MessageBuffer&& right)
	{
		if (this != &right)
		{
			_wpos = right._wpos;
			_rpos = right._rpos;
			_storage = right.Move();
		}

		return *this;
	}
private:
	size_type _wpos;
	size_type _rpos;
	std::vector<uint8> _storage;
};

#endif /* __MESSAGEBUFFER_H_ */

```