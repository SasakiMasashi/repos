using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ServiceModel;
using System.IO;

namespace DesktopCapture
{
    [ServiceContract(Namespace = "http://WCF.CaptureDesktop")]
    public interface IService1
    {
        [OperationContract]
        byte[] getDesktopImageByteArray();

        [OperationContract]
        string getDesktopImageString();
    }

    public partial class CaptureDesktopForm : Form
    {
        private bool IsCancel = true;
        private IService1 proxy = null;

        public CaptureDesktopForm()
        {
            InitializeComponent();

            ChangeButtonEnable(true);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string ipAddr = IPTextBox.Text;
            EndpointAddress epAddr = new EndpointAddress("http://" + ipAddr + ":8000/WCF/CaptureDesktop");

            BasicHttpBinding bhBinding = new BasicHttpBinding();
            bhBinding.MaxReceivedMessageSize = 10000000;
            proxy = ChannelFactory<IService1>.CreateChannel(bhBinding, epAddr);

            IsCancel = false;
            timer1.Start();

            ChangeButtonEnable(false);
        }

        private void ChangeButtonEnable(bool isStart)
        {
            StartButton.Enabled = isStart;
            StopButton.Enabled = !isStart;
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            timer1.Stop();
            IsCancel = true;

            ChangeButtonEnable(true);
        }

        private void Cancelbutton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (IsCancel || proxy == null)
            {
                return;
            }

            try
            {
                byte[] byteArray = proxy.getDesktopImageByteArray();
                Bitmap bmp = BitmapHelper.ToBitMap(byteArray);
                this.pictureBox.Image = bmp;
            }
            catch (Exception ex)
            {
                IsCancel = true;
                timer1.Stop();
                ChangeButtonEnable(true);
                MessageBox.Show("サービス呼び出し失敗：" + ex.Message);
            }
        }
    }

    public static class BitmapHelper
    {
        public static byte[] ToByteArray(Bitmap bmp)
        {
            MemoryStream ms = new MemoryStream();
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            return ms.GetBuffer();
        }

        public static Bitmap ToBitMap(byte[] byteArray)
        {
            Bitmap bmp;
            using (MemoryStream ms = new MemoryStream(byteArray))
            {
                bmp = new Bitmap(ms);
            }
            return bmp;
        }

        public static Bitmap ToBitMap(string str)
        {
            byte[] byteArray = Encoding.Unicode.GetBytes(str);
            return ToBitMap(byteArray);
        }

        public static string ToString(Bitmap bmp)
        {
            MemoryStream ms = new MemoryStream();
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            return Encoding.Unicode.GetString(ms.ToArray());
        }
    }
}
