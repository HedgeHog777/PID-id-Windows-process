#include <functional>
#include <iostream>
#include <windows.h>
#include <psapi.h>

using PIDVector=vector<DWORD>;
using HMODULEVector=vector<HMODULE>;
template<typename T> bool autoFill(T& vec,std::function<WINBOOL(typename T::value_type*, DWORD, DWORD*)> filler)
{
    //вычисляем размер одного элемента массива
    constexpr auto itmSize=sizeof(typename T::value_type);
    while(true)
    {
        DWORD cnt;
        const auto status=filler(vec.data(), vec.size()*itmSize, &cnt);
        if(!status)
        {
            //в случае ошибки, выходим
            return false;
        }

        //нам приходит размер данных в байтах, а нужно - в штуках
        const auto nItems=cnt/itmSize; 

        if(nItems>=vec.size())
        {
            //если буфер мал - увеличить
            vec.resize( nItems + 1024);
        }
        else
        {
            vec.resize( nItems );
            return true;
        }