#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include <functional>
#include <type_traits>

#include "../mockito4cpp/MethodMock.h"
#include "../mockito4cpp/Clouses.h"

template <typename R, typename... arglist>
struct FunctionWhenClouseImpl :
	public FirstFunctionWhenClouse<R, arglist...>,
	public NextFunctionWhenClouse<R, arglist...>
{

	FunctionWhenClouseImpl(MethodCallMock<R, arglist...> * invocationMock) :
		invocationMock(invocationMock)
	{
		ThenDo([](...)->R{
			return DefaultValue::value<R>(); 
		});
	}

	~FunctionWhenClouseImpl(){}

	NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		invocationMock->appendDo(method);
		return *this;
	}

	NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		invocationMock->clear();
		invocationMock->appendDo(method);
		return *this;
	}
	
private:
	MethodCallMock<R, arglist...>* invocationMock;
};

template <typename R, typename... arglist>
struct StubFunctionClouseImpl : public StubFunctionClouse<R, arglist...> {
	StubFunctionClouseImpl(MethodMock<R, arglist...>* methodMock) : methodMock(methodMock) {
	}

	FirstFunctionWhenClouse<R, arglist...>& When(const arglist&... args) override {
		MethodCallMock<R, arglist...> * invocationMock = methodMock->stubMethodCall(args...);
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>
			(invocationMock);
		return *whenClouse;
	}

	NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>(methodMock->last());
		whenClouse->Do(method);
		return *whenClouse;
	}

private:
	MethodMock<R, arglist...>* methodMock;
};


template <typename... arglist>
struct ProcedureWhenClouseImpl :
	public FirstProcedureWhenClouse<arglist...>,
	public NextProcedureWhenClouse<arglist...>{

		ProcedureWhenClouseImpl(MethodCallMock<void, arglist...>* invocationMock) :
			invocationMock(invocationMock)
		{
			ThenReturn();
		}

		virtual ~ProcedureWhenClouseImpl() {}

		NextProcedureWhenClouse<arglist...>& ThenDo(std::function<void(arglist...)> method) override {
			invocationMock->appendDo(method);
			return *this;
		}

		NextProcedureWhenClouse<arglist...>& Do(std::function<void(arglist...)> method) override {
			invocationMock->clear();
			invocationMock->appendDo(method);
			return *this;
		}
		
private:
	MethodCallMock<void, arglist...>* invocationMock;
};

template <typename... arglist>
struct StubProcedureClouseImpl : public StubProcedureClouse<arglist...> {
	StubProcedureClouseImpl(MethodMock<void, arglist...>* methodMock) : methodMock(methodMock) {
	}

	FirstProcedureWhenClouse<arglist...>& When(const arglist&... args) override {
		MethodCallMock<void, arglist...> * invocationMock = methodMock->stubMethodCall(args...);
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(invocationMock);
		return *whenClouse;
	};

	NextProcedureWhenClouse<arglist...>& Do(std::function<void(arglist...)> method) override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(methodMock->last());
		whenClouse->Do(method);
		return *whenClouse;
	}
	
private:
	MethodMock<void, arglist...>* methodMock;
};

template <typename... arglist>
struct StubDataMemberClouseImpl : public StubDataMemberClouse<arglist...> {
	virtual void operator()(std::initializer_list<arglist...> list) override {
	
	};
};
#endif // ClousesImpl_h__