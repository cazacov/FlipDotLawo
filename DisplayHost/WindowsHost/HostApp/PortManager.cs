using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HostApp
{
    internal class PortManager
    {
        private SerialPort port;
        public event EventHandler<ReceiveEventArgs> DataReceived;
        public bool IsConnected { get; private set; }

        public PortManager()
        {
            this.port = null;
        }

        protected virtual void OnDataReceived(ReceiveEventArgs e)
        {
            var handler = DataReceived;
            handler?.Invoke(this, e);
        }

        public string[] ListPorts()
        {
            return SerialPort.GetPortNames();
        }

        public bool TryConnect(string portName)
        {
            if (this.port != null)
            {
                port.DataReceived -= PortDataReceived;
                port.Dispose();
            }

            try
            {

                this.port = new SerialPort(portName, 115200);
                this.port.Open();
                port.DataReceived += PortDataReceived;
                port.WriteLine("");
            }
            catch (Exception ex)
            {
                OnDataReceived(new ReceiveEventArgs() { Text = $"{ex.GetType().Name}: {ex.Message}" });
                IsConnected = false;
                if (port != null)
                {
                    port.Dispose();
                    port = null;
                }
                return false;
            }

            port.WriteLine("m");
            port.WriteLine("4");
            port.WriteLine("3");
            Thread.Sleep(100);
            port.WriteLine("P");
            return true;
        }

        private void PortDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            this.IsConnected = true;
            SerialPort spL = (SerialPort)sender;
            byte[] buf = new byte[spL.BytesToRead];
            spL.Read(buf, 0, buf.Length);
            String txt = String.Empty;
            foreach (var b in buf)
            {
                char ch = (char)b;
                txt += ch;
            }
            OnDataReceived(new ReceiveEventArgs() { Text = txt});
        }
    }

    public class ReceiveEventArgs : EventArgs
    {
        public string Text { get; set; }
    }
}
