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
        private int displayWidth;
        private int displayHeight;

        public Player(FrameManager frameManager, FrameRenderer frameRenderer)
        {
            this.frameManager = frameManager;
            this.frameRenderer = frameRenderer;
            this.timer = new Timer();
            this.timer.Tick += LoadNextFrame;

        }

        private void LoadNextFrame(object? sender, EventArgs e)
        {
            this.timer.Stop();
            frameNr++;

            var fileName = Path.Combine(path, $"frame{frameNr:00000}.png");
            if (File.Exists(fileName))
            {
                var frame = frameManager.LoadFrame(fileName, displayWidth, displayHeight);
                OnFrameLoaded(new FrameEventArgs() { FrameNo = frameNr, Frame = frame});
            }
            else
            {
                stopFlag = true;
            }
            if (!stopFlag)
            {
                this.timer.Start();
            }
            else
            {
                OnPlayFinished(EventArgs.Empty);
            }
        }

        public void Play(string path, int displayWidth, int displayHeight)
        {
            this.stopFlag = false;
            this.timer.Interval = 33;
            frameNr = 0;
            this.path = path;
            this.displayWidth = displayWidth;
            this.displayHeight = displayHeight;
            this.timer.Start();
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
