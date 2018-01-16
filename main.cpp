/*
 * main.cpp
 *
 *  Created on: 10 sty 2018
 *      Author: miecznik
 */

#include <iostream>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


using namespace std;

class testclass{
/*
 * object type to be used as an examle
 */
private:
	int priv;
public:
	int val;
	testclass(int, int);
	void increase(){
		val++;
	}
	int get(){
		return priv;
	}
	int priv_sum(int a){
			return a+priv;
	}
	static void puts(const char* str){
		cout << "dbg: " << str << endl;
	}
};

testclass::testclass(int a, int b): priv(a), val(b){
}


template <class T>
class exec_tracer{
/*
 * This class is supposed to give name for called object and trace its execution
 * Add some standard dbg output with time and full object name

 * Usage:
 * define such function
 * int sum(int a, int b){
 * 		return a+b;
 * }
 * and define exec_tracer object with name sum:
 * exec_tracer<int(&)(int,int)> f(sum, (char*)"sum");
 * it is template of refernce to functin which accepts args: int, int and returns int

 * Exemaple with pointer to function:
 * 	int (*f_ptr)(int, int) = sum;
 *	exec_tracer<int(*)(int,int)> f(f_ptr, (char*)"sum");
 *
 *	Example with int:
 *	int a=10;
 *	exec_tracer<int> a_traced(a, "a");
 *	++a_traced.obj_handler	//will increase "int a" beceause it is reference.
 *
 *	Execute or call traced object by:
 *	f.obj_handler(args if any)
 *
 *	TODO: create some macro so you can call it like:
 *	TRACE int a=10;
 *	or
 *	TRACE a;
 *	try to workout how to trace slected methods in classes
 */
private:
	static char object_tracer_welcome[];
	static uint16_t instance_id;
	char name[10];
public:
	T& obj_handler;
	exec_tracer(T&, const char*);
	exec_tracer(T&, const char*, void (&print_func)(const char*));
	~exec_tracer();
};

template <class T>
char exec_tracer<T>::object_tracer_welcome[] = "Object tracer:\n";

template <class T>
uint16_t exec_tracer<T>::instance_id;

template <class T>
exec_tracer<T>::exec_tracer(T& obj, const char* n): obj_handler(obj){
	strcpy(name, n);	//maybe default name coud be an address ?
	instance_id++;		//instead of id maybe address of object can be applied
	cout << object_tracer_welcome;
	cout << "Started instance of " << name << "_" << instance_id;
	cout << ", at address 0x" << hex << (int)&obj_handler << endl;
}

template <class T>
exec_tracer<T>::exec_tracer(T& obj, const char* n, void (&print_func)(const char*)):obj_handler(obj){
	strcpy(name, n);	//maybe default name coud be an address ?
	instance_id++;		//instead of id maybe address of object can be applied
	print_func(object_tracer_welcome);
	print_func("Started instance of ");
	print_func(name);
	print_func("_");
	//print_func(instance_id);
	//cout << ", at address 0x" << hex << (int)&obj_handler << endl;
}

template <class T>
exec_tracer<T>::~exec_tracer(){
	cout << object_tracer_welcome;
	cout << "Destroyed instance of " << name << "_" << instance_id <<endl;
}

int sum(int a, int b){
	return a+b;
}

void _puts(const char* str){
	cout << "puts " << str << endl;
}

int main(){
	testclass tc(3,4);
	cout << "address " << &tc << endl;
	exec_tracer<testclass> traced_class(tc, "tclass", testclass::puts);

	//cout << traced_class.obj_handler.priv_sum(5) << endl;
	cout << tc.val << endl;
	//traced_class.obj_handler.increase();
	//cout << tc.val << endl;
	return 0;
}
