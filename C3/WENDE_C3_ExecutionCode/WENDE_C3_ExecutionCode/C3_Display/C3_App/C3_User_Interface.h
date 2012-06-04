#pragma once
#using <system.drawing.dll>
#include "CPPIConfig.h"
#include "Coordinates.h"

namespace C3_App {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for C3_User_Interface
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	

	public ref class C3_User_Interface : public System::Windows::Forms::Form
	{
	private:

		typedef System::Windows::Forms::Form base;
		static C3_User_Interface^ instance;

	public:
		C3_User_Interface(void)
		{
			LoadStatusInds();
			InitializeComponent();
			instance = this;
			//
			//TODO: Add the constructor code here
			//
		}

		property static C3_User_Interface^ Instance
		{
			C3_User_Interface^ get()
			{
				return instance;
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

		virtual void OnFormClosed(FormClosedEventArgs^ e) override
		{
			base::OnFormClosed(e);
			instance = nullptr;
		}
	private: System::Windows::Forms::FlowLayoutPanel^  pPPIPanel;
	protected: 
		//~C3_User_Interface()
		//{
		//	if (components)
		//	{
		//		delete components;
		//	}
		//}

	protected: 
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	public: System::Windows::Forms::PictureBox^  pbLiveFeed;


	private: System::Windows::Forms::GroupBox^  groupBox4;
	public: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	public: System::Windows::Forms::DataGridView^  dgvDtiLog;

	public: System::Windows::Forms::PictureBox^  pbCameraStatus;




	public: System::Windows::Forms::PictureBox^  pbCameraComms;
	public: System::Windows::Forms::PictureBox^  pbRoverAcq;


	public: System::Windows::Forms::PictureBox^  pbOverallStatus;



	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::PictureBox^  pbLaserActivity;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::PictureBox^  pbLaserStatus;
	public: System::Windows::Forms::PictureBox^  pbLaserComms;

	//public: CPPIConfig^ pPPI;
	public: System::Windows::Forms::Panel^  pPPI;
	public: System::Windows::Forms::Button^  cmdExport;
	private: System::Windows::Forms::GroupBox^  gbAlerts;
	public: System::Windows::Forms::TextBox^  tbAlertsPanel;

	public: System::Drawing::Image ^ OnlineInd;
			System::Drawing::Image ^ OfflineInd;
			System::Drawing::Image ^ UnknownInd;
			System::Drawing::Image ^ UnknownNsInd;
			System::Drawing::Image ^ InactiveInd;
			System::Drawing::Image ^ EnergizedInd;
			System::Drawing::Image ^ AcquiredInd;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Button^  CalibrateButton;
	private: bool IsCalibrated; 

	public: 

	//private: System::Windows::Forms::SaveFileDialog^  dlgExportDTI; //new this on button click.



	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		void LoadStatusInds(void) {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(C3_User_Interface::typeid));
			this->OnlineInd    = System::Drawing::Image::FromFile("Online.png");
			this->OfflineInd   = System::Drawing::Image::FromFile("Offline.png");
			this->UnknownInd   = System::Drawing::Image::FromFile("Unknown.png");
			this->UnknownNsInd = System::Drawing::Image::FromFile("Unknown-NoShield.png");
			this->InactiveInd  = System::Drawing::Image::FromFile("Inactive.png");
			this->EnergizedInd = System::Drawing::Image::FromFile("Energized.png");
			this->AcquiredInd  = System::Drawing::Image::FromFile("Acquired.png");
			delete resources;
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>

		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(C3_User_Interface::typeid));
			this->pPPIPanel = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->pPPI = (gcnew System::Windows::Forms::Panel());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->pbLiveFeed = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->cmdExport = (gcnew System::Windows::Forms::Button());
			this->dgvDtiLog = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pbCameraStatus = (gcnew System::Windows::Forms::PictureBox());
			this->pbCameraComms = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->CalibrateButton = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->pbLaserActivity = (gcnew System::Windows::Forms::PictureBox());
			this->pbRoverAcq = (gcnew System::Windows::Forms::PictureBox());
			this->pbOverallStatus = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->pbLaserStatus = (gcnew System::Windows::Forms::PictureBox());
			this->pbLaserComms = (gcnew System::Windows::Forms::PictureBox());
			this->gbAlerts = (gcnew System::Windows::Forms::GroupBox());
			this->tbAlertsPanel = (gcnew System::Windows::Forms::TextBox());
			this->pPPIPanel->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLiveFeed))->BeginInit();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dgvDtiLog))->BeginInit();
			this->groupBox4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraStatus))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraComms))->BeginInit();
			this->groupBox5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserActivity))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbRoverAcq))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbOverallStatus))->BeginInit();
			this->groupBox6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserStatus))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserComms))->BeginInit();
			this->gbAlerts->SuspendLayout();
			this->SuspendLayout();
			// 
			// pPPIPanel
			// 
			this->pPPIPanel->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->pPPIPanel->Controls->Add(this->groupBox1);
			this->pPPIPanel->Controls->Add(this->groupBox2);
			this->pPPIPanel->Controls->Add(this->groupBox3);
			this->pPPIPanel->Controls->Add(this->groupBox4);
			this->pPPIPanel->Controls->Add(this->groupBox5);
			this->pPPIPanel->Controls->Add(this->groupBox6);
			this->pPPIPanel->Controls->Add(this->gbAlerts);
			this->pPPIPanel->Location = System::Drawing::Point(12, 12);
			this->pPPIPanel->Name = L"pPPIPanel";
			this->pPPIPanel->Size = System::Drawing::Size(1329, 682);
			this->pPPIPanel->TabIndex = 0;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->pPPI);
			this->groupBox1->Location = System::Drawing::Point(3, 3);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(417, 347);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"PPI Display";
			// 
			// pPPI
			// 
			this->pPPI->CausesValidation = false;
			this->pPPI->Location = System::Drawing::Point(6, 19);
			this->pPPI->Name = L"pPPI";
			this->pPPI->Size = System::Drawing::Size(405, 320);
			this->pPPI->TabIndex = 0;
			this->pPPI->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &C3_User_Interface::flowLayoutPanel1_Paint);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->pbLiveFeed);
			this->groupBox2->Location = System::Drawing::Point(426, 3);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(438, 347);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Live Image";
			// 
			// pbLiveFeed
			// 
			this->pbLiveFeed->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbLiveFeed.Image")));
			this->pbLiveFeed->Location = System::Drawing::Point(58, 59);
			this->pbLiveFeed->Name = L"pbLiveFeed";
			this->pbLiveFeed->Size = System::Drawing::Size(330, 224);
			this->pbLiveFeed->TabIndex = 0;
			this->pbLiveFeed->TabStop = false;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->cmdExport);
			this->groupBox3->Controls->Add(this->dgvDtiLog);
			this->groupBox3->Location = System::Drawing::Point(870, 3);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(452, 347);
			this->groupBox3->TabIndex = 1;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"DTI Log";
			// 
			// cmdExport
			// 
			this->cmdExport->Location = System::Drawing::Point(65, 306);
			this->cmdExport->Name = L"cmdExport";
			this->cmdExport->Size = System::Drawing::Size(343, 33);
			this->cmdExport->TabIndex = 1;
			this->cmdExport->Text = L"&Export";
			this->cmdExport->UseVisualStyleBackColor = true;
			this->cmdExport->Click += gcnew System::EventHandler(this, &C3_User_Interface::cmdExport_Click);
			// 
			// dgvDtiLog
			// 
			this->dgvDtiLog->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dgvDtiLog->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column1, this->Column2, 
				this->Column3});
			this->dgvDtiLog->Location = System::Drawing::Point(65, 19);
			this->dgvDtiLog->Name = L"dgvDtiLog";
			this->dgvDtiLog->ReadOnly = true;
			this->dgvDtiLog->Size = System::Drawing::Size(343, 280);
			this->dgvDtiLog->TabIndex = 0;
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"Date/Time";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"DTI (m)";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"PASS / FAIL";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Controls->Add(this->label1);
			this->groupBox4->Controls->Add(this->pbCameraStatus);
			this->groupBox4->Controls->Add(this->pbCameraComms);
			this->groupBox4->Location = System::Drawing::Point(3, 356);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(411, 272);
			this->groupBox4->TabIndex = 1;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Camera";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(85, 123);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(37, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Status";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(85, 42);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(84, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Communications";
			// 
			// pbCameraStatus
			// 
			this->pbCameraStatus->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbCameraStatus.Image")));
			this->pbCameraStatus->Location = System::Drawing::Point(88, 139);
			this->pbCameraStatus->Name = L"pbCameraStatus";
			this->pbCameraStatus->Size = System::Drawing::Size(239, 47);
			this->pbCameraStatus->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbCameraStatus->TabIndex = 1;
			this->pbCameraStatus->TabStop = false;
			// 
			// pbCameraComms
			// 
			this->pbCameraComms->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbCameraComms.Image")));
			this->pbCameraComms->Location = System::Drawing::Point(88, 58);
			this->pbCameraComms->Name = L"pbCameraComms";
			this->pbCameraComms->Size = System::Drawing::Size(239, 47);
			this->pbCameraComms->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbCameraComms->TabIndex = 0;
			this->pbCameraComms->TabStop = false;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->label8);
			this->groupBox5->Controls->Add(this->CalibrateButton);
			this->groupBox5->Controls->Add(this->label7);
			this->groupBox5->Controls->Add(this->label6);
			this->groupBox5->Controls->Add(this->label5);
			this->groupBox5->Controls->Add(this->pbLaserActivity);
			this->groupBox5->Controls->Add(this->pbRoverAcq);
			this->groupBox5->Controls->Add(this->pbOverallStatus);
			this->groupBox5->Location = System::Drawing::Point(420, 356);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(438, 272);
			this->groupBox5->TabIndex = 2;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Demo State";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(242, 237);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(0, 13);
			this->label8->TabIndex = 9;
			// 
			// CalibrateButton
			// 
			this->CalibrateButton->Location = System::Drawing::Point(103, 232);
			this->CalibrateButton->Name = L"CalibrateButton";
			this->CalibrateButton->Size = System::Drawing::Size(239, 29);
			this->CalibrateButton->TabIndex = 8;
			this->CalibrateButton->Text = L"Ready to Calibrate";
			this->CalibrateButton->UseVisualStyleBackColor = true;
			this->CalibrateButton->Click += gcnew System::EventHandler(this, &C3_User_Interface::calibrateButton_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(100, 162);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(66, 13);
			this->label7->TabIndex = 7;
			this->label7->Text = L"Laser Status";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(100, 96);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(69, 13);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Rover Status";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(100, 27);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(85, 13);
			this->label5->TabIndex = 5;
			this->label5->Text = L"WENDE System";
			// 
			// pbLaserActivity
			// 
			this->pbLaserActivity->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbLaserActivity.Image")));
			this->pbLaserActivity->Location = System::Drawing::Point(103, 179);
			this->pbLaserActivity->Name = L"pbLaserActivity";
			this->pbLaserActivity->Size = System::Drawing::Size(239, 47);
			this->pbLaserActivity->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbLaserActivity->TabIndex = 4;
			this->pbLaserActivity->TabStop = false;
			// 
			// pbRoverAcq
			// 
			this->pbRoverAcq->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbRoverAcq.Image")));
			this->pbRoverAcq->Location = System::Drawing::Point(103, 112);
			this->pbRoverAcq->Name = L"pbRoverAcq";
			this->pbRoverAcq->Size = System::Drawing::Size(239, 47);
			this->pbRoverAcq->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbRoverAcq->TabIndex = 3;
			this->pbRoverAcq->TabStop = false;
			// 
			// pbOverallStatus
			// 
			this->pbOverallStatus->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbOverallStatus.Image")));
			this->pbOverallStatus->Location = System::Drawing::Point(103, 43);
			this->pbOverallStatus->Name = L"pbOverallStatus";
			this->pbOverallStatus->Size = System::Drawing::Size(239, 47);
			this->pbOverallStatus->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbOverallStatus->TabIndex = 2;
			this->pbOverallStatus->TabStop = false;
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->label3);
			this->groupBox6->Controls->Add(this->label4);
			this->groupBox6->Controls->Add(this->pbLaserStatus);
			this->groupBox6->Controls->Add(this->pbLaserComms);
			this->groupBox6->Location = System::Drawing::Point(864, 356);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(449, 261);
			this->groupBox6->TabIndex = 3;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Laser";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(103, 124);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(37, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Status";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(103, 43);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(84, 13);
			this->label4->TabIndex = 6;
			this->label4->Text = L"Communications";
			// 
			// pbLaserStatus
			// 
			this->pbLaserStatus->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbLaserStatus.Image")));
			this->pbLaserStatus->Location = System::Drawing::Point(106, 140);
			this->pbLaserStatus->Name = L"pbLaserStatus";
			this->pbLaserStatus->Size = System::Drawing::Size(239, 47);
			this->pbLaserStatus->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbLaserStatus->TabIndex = 5;
			this->pbLaserStatus->TabStop = false;
			// 
			// pbLaserComms
			// 
			this->pbLaserComms->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pbLaserComms.Image")));
			this->pbLaserComms->Location = System::Drawing::Point(106, 59);
			this->pbLaserComms->Name = L"pbLaserComms";
			this->pbLaserComms->Size = System::Drawing::Size(239, 47);
			this->pbLaserComms->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pbLaserComms->TabIndex = 4;
			this->pbLaserComms->TabStop = false;
			// 
			// gbAlerts
			// 
			this->gbAlerts->Controls->Add(this->tbAlertsPanel);
			this->gbAlerts->Location = System::Drawing::Point(3, 634);
			this->gbAlerts->Name = L"gbAlerts";
			this->gbAlerts->Size = System::Drawing::Size(1316, 51);
			this->gbAlerts->TabIndex = 4;
			this->gbAlerts->TabStop = false;
			// 
			// tbAlertsPanel
			// 
			this->tbAlertsPanel->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tbAlertsPanel->Enabled = false;
			this->tbAlertsPanel->Font = (gcnew System::Drawing::Font(L"Courier New", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tbAlertsPanel->ForeColor = System::Drawing::Color::Red;
			this->tbAlertsPanel->Location = System::Drawing::Point(0, 11);
			this->tbAlertsPanel->Multiline = true;
			this->tbAlertsPanel->Name = L"tbAlertsPanel";
			this->tbAlertsPanel->ReadOnly = true;
			this->tbAlertsPanel->Size = System::Drawing::Size(1283, 34);
			this->tbAlertsPanel->TabIndex = 0;
			this->tbAlertsPanel->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// C3_User_Interface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1344, 698);
			this->Controls->Add(this->pPPIPanel);
			this->Name = L"C3_User_Interface";
			this->Text = L"C3_Subsystem";
			this->pPPIPanel->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLiveFeed))->EndInit();
			this->groupBox3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dgvDtiLog))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraStatus))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraComms))->EndInit();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserActivity))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbRoverAcq))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbOverallStatus))->EndInit();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserStatus))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserComms))->EndInit();
			this->gbAlerts->ResumeLayout(false);
			this->gbAlerts->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void flowLayoutPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
				 //extern int roverContactX;
				 //extern int roverContactY;
				 Coordinates * coordsObj = Coordinates::GetCoordinatesHandle();
				 Coordinates::CoordinatePair RoverContact;
				 RoverContact = coordsObj->GetNewCoordinatePair();

				 System::Drawing::Graphics^ g = e->Graphics;
				 g->Clear(Color::White);

				 System::Drawing::Rectangle rect = Form::ClientRectangle;
				 System::Drawing::Rectangle gFailureLine;
				 System::Drawing::Rectangle gPlayingField;

				 // Draw Failure Line

				 gFailureLine.Width = 300;
				 gFailureLine.Height = 300;
				 gFailureLine.X = pPPI->Width / 2 - (gFailureLine.Width / 2);
				 gFailureLine.Y = pPPI->Height / 2 - (gFailureLine.Height / 2);

 				 Pen^ redPen = gcnew Pen(Color::Red);
				 redPen->Width = 2;
				 g->DrawEllipse(redPen, gFailureLine);

				 // Draw Playing Field Line

				 gPlayingField.Width = gFailureLine.Width - 150;
				 gPlayingField.Height = gFailureLine.Height - 150;
				 gPlayingField.X = pPPI->Width / 2 - (gPlayingField.Width / 2);
				 gPlayingField.Y = pPPI->Height / 2 - (gPlayingField.Height / 2);

				 Pen^ bluePen = gcnew Pen(Color::Blue);
				 bluePen->Width = 2;
				 g->DrawEllipse(bluePen, gPlayingField);

				 // Draw Contact (rover)

				 System::Drawing::Image^ roverContact = System::Drawing::Image::FromFile( "delta.png" );
				 int x = pPPI->Width / 2 - (roverContact->Width / 2) - RoverContact.x;
				 int y = pPPI->Height / 2 - (roverContact->Height / 2) - RoverContact.y;
				 //int x = pPPI->Width / 2 - (roverContact->Width / 2) - roverContactX;
				 //int y = pPPI->Height / 2 - (roverContact->Height / 2) - roverContactY;
				 g->DrawImage(roverContact, x, y);
				 
			 }	  

private: System::Void cmdExport_Click(System::Object^  sender, System::EventArgs^  e) {
			 //dlgExportDTI->ShowDialog();
			 //create a stream we can use to write to
			 System::IO::FileStream^ srmOutput;
			 //create strings as needed
			 String^ rowEntry = "";
			 //create an encoder we can use for writing
			 System::Text::UnicodeEncoding uniEncoding;
			 //create a new SaveFileDialog
			 SaveFileDialog^ dlgExportDti = gcnew SaveFileDialog;
			 //filter for CSV files, and set the filter to be the default one.
			 dlgExportDti->Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
			 dlgExportDti->FilterIndex = 1;
			 //set the file dialog to My Documents
			 dlgExportDti->InitialDirectory = Environment::GetFolderPath(Environment::SpecialFolder::MyDocuments);
			 //call ShowDialog. This won't return until the user does something. If the user clicks OK...
			 if ( dlgExportDti->ShowDialog() == System::Windows::Forms::DialogResult::OK )
			 {
				 //...open the file for writing, provided we can handle it.
				 if ( (srmOutput = safe_cast<System::IO::FileStream^>(dlgExportDti->OpenFile())) != nullptr )
				 {
					 //Put in the first row for the CSV file
					 String^ topRow = String::Concat(
						 Column1->HeaderText, ",",
						 Column2->HeaderText, ",",
						 Column3->HeaderText, ",",
						 "\n");
					 //String^ topRow = "Trial Number,DTI (m),TTI (s),Pass/Fail,\n";
					 srmOutput->Write( uniEncoding.GetBytes(topRow), 0, uniEncoding.GetByteCount(topRow));
					 //loop through each row in the DtiLog
					 for (int i = 0; i < (dgvDtiLog->RowCount - 1); i++)
					 {
						 int trialNum = i+1;
						 //for each row, pull out the data
						 rowEntry = String::Concat(
							 trialNum.ToString(), ",",		//Trial Number
							 dgvDtiLog[0,i]->Value, ",",	//Date/Time
							 dgvDtiLog[1,i]->Value, ",",	//DTI (m)
							 dgvDtiLog[2,i]->Value, ",",	//Pass/Fail
							 "\n");							//Newline
						 srmOutput->Write( uniEncoding.GetBytes(rowEntry), 0, uniEncoding.GetByteCount(rowEntry));
					 }
					 //close the file pointer when it's done
					 srmOutput->Close();
				 }
			 }
			 //TODO: Add set the bool for unexported data to FALSE
			 delete dlgExportDti;
		 }
private: System::Void calibrateButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->CalibrateButton->Text = "Calibrating...";
			 this->CalibrateButton->Enabled = false;
			 System::Threading::Thread::Sleep(5000);
			 if (!IsCalibrated) {
				 this->CalibrateButton->Text = "Calibration Failed.";
				 IsCalibrated = true;
				 this->CalibrateButton->Enabled = true;
			 }
			 else {
				 this->CalibrateButton->Text = "Calibration Success!";
				 this->CalibrateButton->Enabled = false;
			 }
		 }
};
}


