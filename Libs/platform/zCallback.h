#pragma once
//�ص�������ģ��

template <typename T, typename RTValue = bool>
struct zCallback
{
	virtual RTValue exec(T* e) = 0;
	virtual ~zCallback() {}
};

//ɾ�������ӿ�
template <typename T>
struct zRemoveIf
{
	//��ɾ����entity�洢������
	std::vector<T*> removed;
	virtual bool isIt(T* entity) = 0;
	virtual ~zRemoveIf() {}
};
