
#include <iostream>
using namespace std;

class box
{
	public:
		int get_box_soc(void);
		void set_box_soc(int len,int with,int higth);
		friend void get_test(box *x);
		box();
		~box();
	private:
	int length;
	int with;
	int higth;
};
//构造函数,在一个类创建时调用,该函数可以对类成员做一些初始化
box::box(void)
{
	cout << "create box" << endl;
}
//析构函数,删除一个对象时调用,一般是释放资源
box::~box(void)
{
	cout << "delete box" << endl;
}

int box::get_box_soc(void)
{
	return  length + with + higth;
}

void box::set_box_soc(int len,int w,int h)
{
	cout << "set data" << endl;
	length = len;
	with = w;
	higth = h;
	return ;
}

// 继承(nbox 继承了 box,即 nbox 为box的派生类，基类为 box)
class nbox:public box
{
	public:
		int get_box_soc(void);
		int set_nbox_soc(int m);
	private:
		int mac;
};

int nbox::set_nbox_soc(int m)
{
	mac = m;
}
int nbox::get_box_soc(void)
{
	return mac ;
}
// 友元函数/类，不属于类的成员，但可以访问类的所有成员
void get_test(box *p)
{
	cout << p->length << endl;
	cout << p->with << endl;
	cout << p->higth << endl;
}

int main(void)
{
	box *pbox = new box;
	nbox newbox;
	if(pbox)
	{
		pbox->set_box_soc(10,4,5);
		cout << pbox->get_box_soc()<<endl;
		get_test(pbox);
		delete pbox;
	}
	newbox.set_box_soc(10,6,9);
	newbox.set_nbox_soc(1000);
	cout << newbox.get_box_soc() << endl;
	return 0;
}
