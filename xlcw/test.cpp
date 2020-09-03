// 暴力查找
int ViolentMatch(char* s, char* p)  
{  
    int sLen = strlen(s);  
    int pLen = strlen(p);  
  
    int i = 0;  
    int j = 0;  
    while (i < sLen && j < pLen)  
    {  
        if (s[i] == p[j])  
        {  
            //①如果当前字符匹配成功（即S[i] == P[j]），则i++，j++      
            i++;  
            j++;  
        }  
        else  
        {  
            //②如果失配（即S[i]! = P[j]），令i = i - (j - 1)，j = 0      
            i = i - j + 1;  
            j = 0;  
        }  
    }  
    //匹配成功，返回模式串p在文本串s中的位置，否则返回-1  
    if (j == pLen)  
        return i - j;  
    else  
        return -1;  
}  

#include<stdlib.h>
#include<vector>
using namespace std;
inline void NEXT(const string&T, vector<int>& next){//按模式串生成vector,next(T.size())
    next[0] = -1;
    for (int i = 1; i<T.size(); i++)
    {
        int j = next[i - 1];
        while (j >= 0 && T[i - 1] != T[j])//递推计算
        {
            j = next[j];
        }
        if (j >= 0 &&  T[i - 1] == T[j]) 
            next[i] = j + 1;
        else 
            next[i] = 0;
    }
}
inline string::size_type COUNT_KMP(const string&S, const string&T){
    //利用模式串T的next函数求T在主串S中的个数count的KMP算法
    //其中T非空，
    vector<int> next(T.size());
    NEXT(T, next);
    string::size_type index, count = 0;
    for (index = 0; index<S.size(); ++index){
        int pos = 0;
        string::size_type iter = index;
        while (pos<T.size() && iter<S.size()){
            if (S[iter] == T[pos]){ ++iter; ++pos; }
            else{
                if (pos == 0) ++iter;
                else pos = next[pos - 1] + 1;
            }
        }
        if (pos == T.size() && (iter - index) == T.size()) ++count;
    }
    return count;
}
  
int main(int argc, char*argv[])
{
    string S="abaabbgcabagbbgbgabbcabaabcacabaabcabgbgbgcabaabcacabaabcac";
    string T="bgbgbg"; 
    //cin >> S;
    //cin >> T;
    string::size_type count = COUNT_KMP(S, T);
    cout << count << endl;
    system("PAUSE");
    return 0;
}

template<int BYTE>
class CMask
{
public:
    CMask()
    {
        _mask = {0x00};
    }
    // 取某一位的值
    bool operator[](const int& pos)
    {
        if(pos >= BYTE*8)
        {
            return false;
        }
        int iByte = pos / 8;
        int iPos = pos % 8;
        return _mask[iByte] & (0x01 << iPos);
    }
    // 将某一位置1
    void operator<<(const int& pos)
    {
        if(pos >= BYTE*8)
        {
            return;
        }
        int iByte = pos / 8;
        int iPos = pos % 8;
        _mask[iByte] |= (0x01 << iPos);
    }
    // 将某一位置0
    void operator>>(const int& pos)
    {
        if(pos >= BYTE*8)
        {
            return;
        }
        int iByte = pos / 8;
        int iPos = pos % 8;
        _mask[iByte] &= ~(0x01 << iPos);
    }
    // 是否没有子叶节点
    bool Zero()
    {
        for(int i(0); i < BYTE; ++i)
        {
            if(_mask[i] != 0)
            {
                return false;
            }
        }
        return true;
    }
private:
    uint8_t _mask[BYTE];
}

class CTireNode
{
public:
    CTireNode()
    {
    }
    void push()
private:
    //子叶节点 second如果是NULL,就代表这个节点是终节点
    // 一旦这个节点下有新的子节点的时候再创建新的实例
    vector<pair<char, CTireNode*>> _nodes; 
}