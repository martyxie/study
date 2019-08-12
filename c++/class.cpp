
#include <iostream>
using namespace std;

class box
{
	public:
		int get_box_soc(void);
		void set_box_soc(int len,int with,int higth);
		box();
		~box();
	private:
	int length;
	int with;
	int higth;
};

box::box(void)
{
	cout << "create box" << endl;
}
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

int main(void)
{
	box *pbox = new box;
	nbox newbox;
	if(pbox)
	{
		pbox->set_box_soc(10,4,5);
		cout << pbox->get_box_soc()<<endl;
		delete pbox;
	}
	newbox.set_box_soc(10,6,9);
	newbox.set_nbox_soc(1000);
	cout << newbox.get_box_soc() << endl;
	return 0;
}
