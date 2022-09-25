using Timer = System.Windows.Forms.Timer;

namespace HostApp
{
    internal class Player
    {
        private readonly FrameManager frameManager;
        private readonly FrameRenderer frameRenderer;
        private readonly Timer timer;
        private int frameNr = 0;
        private bool stopFlag;
        private string path;
        private bool isProcessing = false;
        private Size displaySize;
        private bool useTimer;

        public Player(FrameManager frameManager, FrameRenderer frameRenderer)
        {
            this.frameManager = frameManager;
            this.frameRenderer = frameRenderer;
            this.timer = new Timer();
            this.timer.Tick += LoadNextFrame;

        }

        private void LoadNextFrame(object? sender, EventArgs e)
        {
            if (isProcessing)
            {
                return;
            }

            isProcessing = true;
            frameNr++;

            if (!stopFlag)
            {
                var fileName = Path.Combine(path, $"frame{frameNr:00000}.png");
                if (File.Exists(fileName))
                {
                    var frame = frameManager.LoadFrame(fileName, displaySize);
                    OnFrameLoaded(new FrameEventArgs() { FrameNo = frameNr, Frame = frame });
                }
                else
                {
                    stopFlag = true;
                }
            }

            if (stopFlag)
            {
                this.timer.Stop();
                OnPlayFinished(EventArgs.Empty);
            }
            isProcessing = false;
        }

        public void Play(string path, Size displaySize, bool useTimer)
        {
            this.useTimer = useTimer;
            this.stopFlag = false;
            this.timer.Interval = 33;
            frameNr = 0;
            this.path = path;
            this.displaySize = displaySize;
            if (!useTimer)
            {
                this.timer.Interval = 1;
            }
            this.timer.Start();
            isProcessing = false;
        }

        public event EventHandler<FrameEventArgs> FrameLoaded;
        public event EventHandler<EventArgs> PlayFinished;

        protected virtual void OnFrameLoaded(FrameEventArgs e)
        {
            var handler = FrameLoaded;
            handler?.Invoke(this, e);
        }

        protected virtual void OnPlayFinished(EventArgs e)
        {
            var handler = PlayFinished;
            handler?.Invoke(this, e);
        }

        public void Stop()
        {
            this.stopFlag = true;
        }
    }

    public class FrameEventArgs : EventArgs
    {
        public int FrameNo;
        public Frame Frame;
    }
}
