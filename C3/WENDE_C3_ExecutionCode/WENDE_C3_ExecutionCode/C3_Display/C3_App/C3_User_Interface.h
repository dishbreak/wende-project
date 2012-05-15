#pragma once
#using <system.drawing.dll>


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
		//~C3_User_Interface()
		//{
		//	if (components)
		//	{
		//		delete components;
		//	}
		//}
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	protected: 
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;

	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	public: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	public: System::Windows::Forms::PictureBox^  pbCameraStatus;




	private: System::Windows::Forms::PictureBox^  pbCameraComms;

	private: System::Windows::Forms::PictureBox^  pictureBox5;
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

	private: System::Windows::Forms::PictureBox^  pictureBox8;
	private: System::Windows::Forms::Panel^  pPPI;
	private: System::Windows::Forms::Button^  cmdExport;
	private: System::Windows::Forms::GroupBox^  gbAlerts;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::SaveFileDialog^  dlgExportDTI;



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(C3_User_Interface::typeid));
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->pPPI = (gcnew System::Windows::Forms::Panel());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->cmdExport = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pbCameraStatus = (gcnew System::Windows::Forms::PictureBox());
			this->pbCameraComms = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->pbLaserActivity = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->pbOverallStatus = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->pbLaserStatus = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox8 = (gcnew System::Windows::Forms::PictureBox());
			this->gbAlerts = (gcnew System::Windows::Forms::GroupBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->dlgExportDTI = (gcnew System::Windows::Forms::SaveFileDialog());
			this->flowLayoutPanel1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->groupBox4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraStatus))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraComms))->BeginInit();
			this->groupBox5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserActivity))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbOverallStatus))->BeginInit();
			this->groupBox6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserStatus))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox8))->BeginInit();
			this->gbAlerts->SuspendLayout();
			this->SuspendLayout();
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->groupBox1);
			this->flowLayoutPanel1->Controls->Add(this->groupBox2);
			this->flowLayoutPanel1->Controls->Add(this->groupBox3);
			this->flowLayoutPanel1->Controls->Add(this->groupBox4);
			this->flowLayoutPanel1->Controls->Add(this->groupBox5);
			this->flowLayoutPanel1->Controls->Add(this->groupBox6);
			this->flowLayoutPanel1->Controls->Add(this->gbAlerts);
			this->flowLayoutPanel1->Location = System::Drawing::Point(12, 12);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(1329, 682);
			this->flowLayoutPanel1->TabIndex = 0;
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
			this->pPPI->Location = System::Drawing::Point(6, 19);
			this->pPPI->Name = L"pPPI";
			this->pPPI->Size = System::Drawing::Size(405, 320);
			this->pPPI->TabIndex = 0;
			this->pPPI->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &C3_User_Interface::flowLayoutPanel1_Paint);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->pictureBox2);
			this->groupBox2->Location = System::Drawing::Point(426, 3);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(438, 347);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Live Image";
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(58, 59);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(330, 224);
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->cmdExport);
			this->groupBox3->Controls->Add(this->dataGridView1);
			this->groupBox3->Location = System::Drawing::Point(870, 3);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(452, 347);
			this->groupBox3->TabIndex = 1;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Performance Metrics";
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
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column1, 
				this->Column2, this->Column3});
			this->dataGridView1->Location = System::Drawing::Point(65, 19);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->Size = System::Drawing::Size(343, 280);
			this->dataGridView1->TabIndex = 0;
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"DTI (m)";
			this->Column1->Name = L"Column1";
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"TTI (s)";
			this->Column2->Name = L"Column2";
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"PASS / FAIL";
			this->Column3->Name = L"Column3";
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Controls->Add(this->label1);
			this->groupBox4->Controls->Add(this->pbCameraStatus);
			this->groupBox4->Controls->Add(this->pbCameraComms);
			this->groupBox4->Location = System::Drawing::Point(3, 356);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(417, 261);
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
			this->groupBox5->Controls->Add(this->label7);
			this->groupBox5->Controls->Add(this->label6);
			this->groupBox5->Controls->Add(this->label5);
			this->groupBox5->Controls->Add(this->pbLaserActivity);
			this->groupBox5->Controls->Add(this->pictureBox5);
			this->groupBox5->Controls->Add(this->pbOverallStatus);
			this->groupBox5->Location = System::Drawing::Point(426, 356);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(438, 261);
			this->groupBox5->TabIndex = 2;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Demo State";
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
			// pictureBox5
			// 
			this->pictureBox5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox5.Image")));
			this->pictureBox5->Location = System::Drawing::Point(103, 112);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(239, 47);
			this->pictureBox5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox5->TabIndex = 3;
			this->pictureBox5->TabStop = false;
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
			this->groupBox6->Controls->Add(this->pictureBox8);
			this->groupBox6->Location = System::Drawing::Point(870, 356);
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
			// pictureBox8
			// 
			this->pictureBox8->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox8.Image")));
			this->pictureBox8->Location = System::Drawing::Point(106, 59);
			this->pictureBox8->Name = L"pictureBox8";
			this->pictureBox8->Size = System::Drawing::Size(239, 47);
			this->pictureBox8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox8->TabIndex = 4;
			this->pictureBox8->TabStop = false;
			// 
			// gbAlerts
			// 
			this->gbAlerts->Controls->Add(this->textBox1);
			this->gbAlerts->Location = System::Drawing::Point(3, 623);
			this->gbAlerts->Name = L"gbAlerts";
			this->gbAlerts->Size = System::Drawing::Size(1316, 51);
			this->gbAlerts->TabIndex = 4;
			this->gbAlerts->TabStop = false;
			// 
			// textBox1
			// 
			this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox1->Enabled = false;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Courier New", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox1->ForeColor = System::Drawing::Color::Red;
			this->textBox1->Location = System::Drawing::Point(7, 11);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(1283, 34);
			this->textBox1->TabIndex = 0;
			this->textBox1->Text = L"ALERT: CONTACT HAS LEFT THE PLAYING FIELD - INVESTIGATE IMMEDIATELY!";
			this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// C3_User_Interface
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1344, 698);
			this->Controls->Add(this->flowLayoutPanel1);
			this->Name = L"C3_User_Interface";
			this->Text = L"C3_Subsystem";
			this->flowLayoutPanel1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->groupBox3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraStatus))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbCameraComms))->EndInit();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserActivity))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbOverallStatus))->EndInit();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbLaserStatus))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox8))->EndInit();
			this->gbAlerts->ResumeLayout(false);
			this->gbAlerts->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void flowLayoutPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
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

				 int nTriangleCenterX = pPPI->Width / 2;
				 int nTriangleCenterY = pPPI->Height / 2;

				 Point point1 = Point(nTriangleCenterX - 10, nTriangleCenterY + 10);
				 Point point2 = Point(nTriangleCenterX + 10, nTriangleCenterY + 10);
				 Point point3 = Point(nTriangleCenterX, nTriangleCenterY - 10);

				 Pen^ greenPen = gcnew Pen(Color::Green);
				 greenPen->Width = 4;

				 array<Point>^ Triangle = {point1, point2, point3};
				 g->DrawPolygon(greenPen, Triangle);
				 
			 }	  

private: System::Void cmdExport_Click(System::Object^  sender, System::EventArgs^  e) {
			 dlgExportDTI->ShowDialog();
		 }
};
}

