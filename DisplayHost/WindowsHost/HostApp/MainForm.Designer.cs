namespace HostApp
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.txtLog = new System.Windows.Forms.TextBox();
            this.cbPorts = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.cbSize = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.pblLog = new System.Windows.Forms.Panel();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.pbFrame = new System.Windows.Forms.PictureBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.pbPreview = new System.Windows.Forms.PictureBox();
            this.btLoad = new System.Windows.Forms.Button();
            this.pblLog.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbFrame)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbPreview)).BeginInit();
            this.SuspendLayout();
            // 
            // txtLog
            // 
            this.txtLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtLog.Location = new System.Drawing.Point(30, 88);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.Size = new System.Drawing.Size(1756, 327);
            this.txtLog.TabIndex = 0;
            // 
            // cbPorts
            // 
            this.cbPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbPorts.FormattingEnabled = true;
            this.cbPorts.Location = new System.Drawing.Point(286, 25);
            this.cbPorts.Name = "cbPorts";
            this.cbPorts.Size = new System.Drawing.Size(242, 40);
            this.cbPorts.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(46, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(229, 32);
            this.label1.TabIndex = 2;
            this.label1.Text = "Bus Pirate COM Port";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(556, 19);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(150, 46);
            this.button1.TabIndex = 3;
            this.button1.Text = "Connect";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // cbSize
            // 
            this.cbSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSize.FormattingEnabled = true;
            this.cbSize.Items.AddRange(new object[] {
            "28x19",
            "38x28",
            "56x38"});
            this.cbSize.Location = new System.Drawing.Point(185, 16);
            this.cbSize.Name = "cbSize";
            this.cbSize.Size = new System.Drawing.Size(242, 40);
            this.cbSize.TabIndex = 4;
            this.cbSize.SelectedIndexChanged += new System.EventHandler(this.cbSize_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(40, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(96, 32);
            this.label2.TabIndex = 5;
            this.label2.Text = "Display:";
            // 
            // pblLog
            // 
            this.pblLog.BackColor = System.Drawing.SystemColors.ControlLight;
            this.pblLog.Controls.Add(this.cbPorts);
            this.pblLog.Controls.Add(this.txtLog);
            this.pblLog.Controls.Add(this.label1);
            this.pblLog.Controls.Add(this.button1);
            this.pblLog.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pblLog.Location = new System.Drawing.Point(0, 667);
            this.pblLog.Name = "pblLog";
            this.pblLog.Size = new System.Drawing.Size(1814, 437);
            this.pblLog.TabIndex = 6;
            // 
            // splitter1
            // 
            this.splitter1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.splitter1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitter1.Cursor = System.Windows.Forms.Cursors.HSplit;
            this.splitter1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.splitter1.Location = new System.Drawing.Point(0, 651);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(1814, 16);
            this.splitter1.TabIndex = 7;
            this.splitter1.TabStop = false;
            // 
            // pbFrame
            // 
            this.pbFrame.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.pbFrame.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pbFrame.Location = new System.Drawing.Point(30, 78);
            this.pbFrame.Name = "pbFrame";
            this.pbFrame.Size = new System.Drawing.Size(694, 550);
            this.pbFrame.TabIndex = 8;
            this.pbFrame.TabStop = false;
            this.pbFrame.SizeChanged += new System.EventHandler(this.pictureBox1_SizeChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.pbPreview);
            this.panel1.Controls.Add(this.btLoad);
            this.panel1.Controls.Add(this.pbFrame);
            this.panel1.Controls.Add(this.cbSize);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1814, 651);
            this.panel1.TabIndex = 9;
            // 
            // pbPreview
            // 
            this.pbPreview.Location = new System.Drawing.Point(782, 78);
            this.pbPreview.Name = "pbPreview";
            this.pbPreview.Size = new System.Drawing.Size(674, 550);
            this.pbPreview.TabIndex = 10;
            this.pbPreview.TabStop = false;
            // 
            // btLoad
            // 
            this.btLoad.Location = new System.Drawing.Point(1528, 93);
            this.btLoad.Name = "btLoad";
            this.btLoad.Size = new System.Drawing.Size(150, 46);
            this.btLoad.TabIndex = 9;
            this.btLoad.Text = "Load";
            this.btLoad.UseVisualStyleBackColor = true;
            this.btLoad.Click += new System.EventHandler(this.btLoad_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(13F, 32F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1814, 1104);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.splitter1);
            this.Controls.Add(this.pblLog);
            this.Name = "MainForm";
            this.Text = "Frame Sender";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.pblLog.ResumeLayout(false);
            this.pblLog.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbFrame)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbPreview)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private TextBox txtLog;
        private ComboBox cbPorts;
        private Label label1;
        private Button button1;
        private ComboBox cbSize;
        private Label label2;
        private Panel pblLog;
        private Splitter splitter1;
        private PictureBox pbFrame;
        private Panel panel1;
        private Button btLoad;
        private PictureBox pbPreview;
    }
}