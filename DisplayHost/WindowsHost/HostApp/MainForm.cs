using System.IO.Ports;

namespace HostApp
{
    public partial class MainForm : Form
    {
        private FrameManager frameManager;
        private Size displaySize;
        private Frame currentFrame;
        private FrameRenderer frameRenderer;
        private bool isConnected;
        private readonly BusPirateClient busPirateClient;
        private Player player;
        private bool isPlaying;

        public MainForm()
        {
            InitializeComponent();
            this.busPirateClient = new BusPirateClient();
            busPirateClient.DataReceived += BusPirateClientDataReceived;

        }

        private void BusPirateClientDataReceived(object? sender, ReceiveEventArgs e)
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
            this.player = new Player(this.frameManager, this.frameRenderer);
            player.FrameLoaded += (o, args) =>
            {
                ShowFrameSafe(args.FrameNo, args.Frame);
            };
            player.PlayFinished += (o, args) =>
            {
                this.isPlaying = false;
                if (cbExport.Checked)
                {
                    frameRenderer.StopCode();
                }
                ShowFinishedSafe();
            };
        }

        private void ShowFinishedSafe()
        {
            if (btPlay.InvokeRequired)
            {
                btPlay.Invoke((Action)(ShowFinishedSafe));
            }
            else
            {
                btPlay.Text = "Play";
            }
        }

        private void ShowFrameSafe(int frameNo, Frame frame)
        {
            if (lbFrame.InvokeRequired)
            {
                void SafeShowFrame()
                {
                    ShowFrameSafe(frameNo, frame);
                }
                txtLog.Invoke((Action)SafeShowFrame);
            }
            else
            {
                lbFrame.Text = $"Frame: {frameNo}";
                DisplayFrame(frame);
                if (isConnected && frameNo % 10 == 0) {
                    if (frame.Width == 28)
                    {
                        busPirateClient.SendBitmap(frame);
                    }
                    else
                    {
                        busPirateClient.SendBitmapStacked(frame);
                    }
                }

                if (cbExport.Checked)
                {
                    if (frameNo % Int32.Parse(txtFrameRate.Text) == 0)
                    {
                        frameRenderer.AddCode(frame, frameNo);
                    }
                }
            }
        }

        private void UpdateDisplaySize()
        {
            switch (cbSize.SelectedIndex)
            {
                case 0:
                    displaySize = new Size(28, 19);
                    break;
                case 1:
                    displaySize = new Size(38, 28);
                    break;
                case 2:
                    displaySize = new Size(56, 38);
                    break;

            }
        }

        private void FillComPorts()
        {
            cbPorts.Items.Clear();
            foreach (var port in busPirateClient.ListPorts())
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
            isConnected = busPirateClient.TryConnect(portName);
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
            string fileName = @"E:\Git\FlipDotLawo\DisplayHost\input\BadApple\Frames-30fps\frame00256.png";
            LoadFrame(fileName);
        }

        private void LoadFrame(string fileName)
        {
            this.currentFrame = frameManager.LoadFrame(fileName, displaySize);
            DisplayFrame(this.currentFrame);
            if (isConnected)
            {
                if (this.currentFrame.Width == 28)
                {
                    busPirateClient.SendBitmap(this.currentFrame);
                }
                else
                {
                    busPirateClient.SendBitmapStacked(this.currentFrame);
                }
            }
        }

        private void DisplayFrame(Frame frame)
        {
            if (this.pbFrame.Image != null)
            {
                this.pbFrame.Image.Dispose();
                this.pbFrame.Image = null;
            }
            this.pbFrame.Image = frameRenderer.RenderFrame(frame, this.pbFrame.Size);
        }

        private void cbSize_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateDisplaySize();
        }

        private void btPlay_Click(object sender, EventArgs e)
        {
            if (!isPlaying)
            {
                btPlay.Text = "Stop";
                isPlaying = true;
                Application.DoEvents();
                if (cbExport.Checked)
                {
                    frameRenderer.StartCode(IoHelper.ToAbsolutePath(txtExportPath.Text), displaySize);
                }
                player.Play(IoHelper.ToAbsolutePath(tbPath.Text), displaySize, isConnected);
            }
            else
            {
                btPlay.Text = "Stopping...";
                player.Stop();
                Application.DoEvents();
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void tbPath_TextChanged(object sender, EventArgs e)
        {

        }
    }
}