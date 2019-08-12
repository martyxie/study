
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
	length = len;
	with = w;
	higth = h;
	return ;
}

int main(void)
{
	box *pbox = new box;
	if(pbox)
	{
		pbox->set_box_soc(10,4,5);
		cout << pbox->get_box_soc()<<endl;
		delete pbox;
	}
	return 0;
}
