using System.IO.Ports;

namespace HostApp
{
    public partial class MainForm : Form
    {
        private SerialPort port;
        private FrameManager frameManager;
        private int displayWidth;
        private int displayHeight;
        private Frame frame;
        private FrameRenderer frameRenderer;
        private bool isConnected;
        private readonly PortManager portManager;

        public MainForm()
        {
            InitializeComponent();
            this.portManager = new PortManager();
            portManager.DataReceived += PortManager_DataReceived;

        }

        private void PortManager_DataReceived(object? sender, ReceiveEventArgs e)
        {
            this.AddToLogSafe(e.Text);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FillComPorts();
            cbSize.SelectedIndex = 0;
            UpdateDisplaySize();
            this.frameManager = new FrameManager();
            this.frameRenderer = new FrameRenderer();
        }

        private void UpdateDisplaySize()
        {
            switch (cbSize.SelectedIndex)
            {
                case 0:
                    displayWidth = 28;
                    displayHeight = 19;
                    break;
                case 1:
                    displayWidth = 38;
                    displayHeight = 28;
                    break;
                case 2:
                    displayWidth = 56;
                    displayHeight = 38;
                    break;

            }
        }

        private void FillComPorts()
        {
            cbPorts.Items.Clear();
            foreach (var port in portManager.ListPorts())
            {
                cbPorts.Items.Add(port);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (cbPorts.SelectedItem == null)
            {
                return;
            }

            String portName = cbPorts.SelectedItem.ToString();
            if (String.IsNullOrWhiteSpace(portName))
            {
                return;
            }
            isConnected = portManager.TryConnect(portName);
        }

        

        public void AddToLogSafe(string text)
        {
            if (txtLog.InvokeRequired)
            {
                void SafeWrite()
                {
                    AddToLogSafe(text);
                }
                txtLog.Invoke((Action)SafeWrite);
            }
            else
            {
                var lines = txtLog.Lines;
                if (lines.Length > 200)
                {
                    var newLines = lines.Skip(lines.Length - 200);
                    txtLog.Lines = newLines.ToArray();
                }
                txtLog.AppendText(text);
                txtLog.SelectionStart = txtLog.Text.Length;
                txtLog.ScrollToCaret();
                
            }
        }


        private void pictureBox1_SizeChanged(object sender, EventArgs e)
        {
            pbFrame.Width = pbFrame.Height * 496 / 368;
        }

        private void btLoad_Click(object sender, EventArgs e)
        {
            string fileName = @"E:\Git\FlipDotLawo\DisplayHost\Esp32\BadApple\Frames-30fps\frame00256.png";
            LoadFrame(fileName);
        }

        private void LoadFrame(string fileName)
        {
            this.frame = frameManager.LoadFrame(fileName, displayWidth, displayHeight);
            DisplayFrame();
            if (isConnected)
            {

            }
        }

        private void DisplayFrame()
        {
            if (this.pbFrame.Image != null)
            {
                this.pbFrame.Image.Dispose();
            }
            this.pbFrame.Image = frameRenderer.RenderFrame(this.frame, this.pbFrame.Size);
        }

        private void cbSize_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateDisplaySize();
        }

    }
}