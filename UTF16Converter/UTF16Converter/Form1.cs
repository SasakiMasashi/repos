using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace UTF16Converter
{
    public partial class UTF16ConverterForm : Form
    {
        private Dictionary<string, string> toUTFMap = new Dictionary<string, string>(4000);
        private Dictionary<string, string> toJPMap = new Dictionary<string, string>(4000);
        
        public UTF16ConverterForm()
        {
            InitializeComponent();

            InitializeDictionary();
        }

        private void InitializeDictionary()
        {
            ConvertDictionaryCreator.CreateToUtf16Map(toUTFMap);
            ConvertDictionaryCreator.CreateReverseMap(toUTFMap, toJPMap);
        }

        private void toUTF16Button_Click(object sender, EventArgs e)
        {
            string text = jpTextBox.Text;

            string result = string.Empty;

            for (int i = 0; i < text.Length; ++i)
            {
                string key = text[i].ToString();
                if(toUTFMap.ContainsKey(key))
                {
                    if (utf16UCheckBox.Checked)
                    {
                        result += "\\u";
                    }

                    string val = toUTFMap[key];
                    if (smallCharCheckBox.Checked)
                    {
                        val = val.ToLower();
                    }
                    else
                    {
                        val = val.ToUpper();
                    }

                    result += val;
                }
            }

            utf16TextBox.Text = result;
        }

        private string[] spritWord(string text, int wSize)
        {
            List<string> words = new List<string>(20);
            
            for (int p = 0; p < text.Length; p += 4)
            {
                if (wSize == 6)
                {
                    p += 2;
                }

                string word = text.Substring(p, 4);
                words.Add(word);
            }

            return words.ToArray();
        }

        private void toJpButton_Click(object sender, EventArgs e)
        {
            string text = utf16TextBox.Text;
            if (text.Length == 0)
            {
                return;
            }

            bool existU = (text.IndexOf("\\u") == 0 || text.IndexOf("\\U") == 0);
            int splitLength = existU ? 6 : 4;

            string[] words = spritWord(text, splitLength);

            string result = string.Empty;
            for (int i = 0; i < words.Length; ++i)
            {
                string key = words[i].ToUpper();
                if (toJPMap.ContainsKey(key))
                {
                    result += toJPMap[key];
                }
            }

            jpTextBox.Text = result;
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
