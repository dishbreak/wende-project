//#include "C3_User_Interface.h"
//#include "GUI_Control.h"
#include "CDisplayManager.h"
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace C3_App {

	/// <summary>
	/// Summary for Test_Driver
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Test_Driver : public System::Windows::Forms::Form
	{
	public:
		Test_Driver(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Test_Driver()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  cmdSend;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  cboCamera;

	private: System::Windows::Forms::ComboBox^  comboBox3;
	private: System::Windows::Forms::Label^  label3;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->cmdSend = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->cboCamera = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Test"});
			this->comboBox1->Location = System::Drawing::Point(83, 10);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 0;
			// 
			// cmdSend
			// 
			this->cmdSend->Location = System::Drawing::Point(83, 156);
			this->cmdSend->Name = L"cmdSend";
			this->cmdSend->Size = System::Drawing::Size(75, 23);
			this->cmdSend->TabIndex = 1;
			this->cmdSend->Text = L"Send";
			this->cmdSend->UseVisualStyleBackColor = true;
			this->cmdSend->Click += gcnew System::EventHandler(this, &Test_Driver::cmdSend_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(33, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Laser";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(43, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Camera";
			// 
			// cboCamera
			// 
			this->cboCamera->FormattingEnabled = true;
			this->cboCamera->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"ONLINE", L"OFFLINE"});
			this->cboCamera->Location = System::Drawing::Point(83, 57);
			this->cboCamera->Name = L"cboCamera";
			this->cboCamera->Size = System::Drawing::Size(121, 21);
			this->cboCamera->TabIndex = 4;
			// 
			// comboBox3
			// 
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Test"});
			this->comboBox3->Location = System::Drawing::Point(83, 106);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(121, 21);
			this->comboBox3->TabIndex = 5;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 109);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(36, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Rover";
			// 
			// Test_Driver
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(226, 211);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox3);
			this->Controls->Add(this->cboCamera);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->cmdSend);
			this->Controls->Add(this->comboBox1);
			this->Name = L"Test_Driver";
			this->Text = L"Test_Driver";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void cmdSend_Click(System::Object^  sender, System::EventArgs^  e) {
				 int nCameraStatus;

				 if(cboCamera->Text == "ONLINE")
					 nCameraStatus = 1;
				 else if(cboCamera->Text == "OFFLINE")
					 nCameraStatus = 0;
				 
				 CDisplayManager dispman;

				 dispman.Update_Camera_Subsystem_Indicator(nCameraStatus);

				 
			 }
	};
}
