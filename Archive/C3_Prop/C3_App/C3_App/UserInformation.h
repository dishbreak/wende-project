using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;

// MP: This currently isn't used. I tried to implement a wrapper
// class to manage properties set's and get's on the form. Didn't work. 

namespace C3_App
{
	public ref class UserInformation
	{
	public: 
		UserInformation()
		{
			buttonValue = "";
		}

	private:
		String^ buttonValue;

	public:
		property String^ Button
		{
			String^ get()
			{
				return buttonValue;
			}
			void set( String^ value )
			{
				buttonValue = value;
			}

		}
	};
}