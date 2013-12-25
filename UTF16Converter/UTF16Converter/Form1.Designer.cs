namespace UTF16Converter
{
    partial class UTF16ConverterForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.toUTF16Button = new System.Windows.Forms.Button();
            this.toJpButton = new System.Windows.Forms.Button();
            this.utf16UCheckBox = new System.Windows.Forms.CheckBox();
            this.convertTextLabel = new System.Windows.Forms.Label();
            this.utf16TextLabel = new System.Windows.Forms.Label();
            this.closeButton = new System.Windows.Forms.Button();
            this.utf16TextBox = new System.Windows.Forms.TextBox();
            this.jpTextBox = new System.Windows.Forms.TextBox();
            this.smallCharCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // toUTF16Button
            // 
            this.toUTF16Button.Location = new System.Drawing.Point(159, 216);
            this.toUTF16Button.Name = "toUTF16Button";
            this.toUTF16Button.Size = new System.Drawing.Size(75, 23);
            this.toUTF16Button.TabIndex = 1;
            this.toUTF16Button.Text = "↓";
            this.toUTF16Button.UseVisualStyleBackColor = true;
            this.toUTF16Button.Click += new System.EventHandler(this.toUTF16Button_Click);
            // 
            // toJpButton
            // 
            this.toJpButton.Location = new System.Drawing.Point(265, 216);
            this.toJpButton.Name = "toJpButton";
            this.toJpButton.Size = new System.Drawing.Size(75, 23);
            this.toJpButton.TabIndex = 1;
            this.toJpButton.Text = "↑";
            this.toJpButton.UseVisualStyleBackColor = true;
            this.toJpButton.Click += new System.EventHandler(this.toJpButton_Click);
            // 
            // utf16UCheckBox
            // 
            this.utf16UCheckBox.AutoSize = true;
            this.utf16UCheckBox.Checked = true;
            this.utf16UCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.utf16UCheckBox.Location = new System.Drawing.Point(371, 220);
            this.utf16UCheckBox.Name = "utf16UCheckBox";
            this.utf16UCheckBox.Size = new System.Drawing.Size(147, 16);
            this.utf16UCheckBox.TabIndex = 2;
            this.utf16UCheckBox.Text = "utf16文字列に/uを付ける";
            this.utf16UCheckBox.UseVisualStyleBackColor = true;
            // 
            // convertTextLabel
            // 
            this.convertTextLabel.AutoSize = true;
            this.convertTextLabel.Location = new System.Drawing.Point(12, 13);
            this.convertTextLabel.Name = "convertTextLabel";
            this.convertTextLabel.Size = new System.Drawing.Size(65, 12);
            this.convertTextLabel.TabIndex = 3;
            this.convertTextLabel.Text = "変換文字列";
            // 
            // utf16TextLabel
            // 
            this.utf16TextLabel.AutoSize = true;
            this.utf16TextLabel.Location = new System.Drawing.Point(12, 227);
            this.utf16TextLabel.Name = "utf16TextLabel";
            this.utf16TextLabel.Size = new System.Drawing.Size(75, 12);
            this.utf16TextLabel.TabIndex = 3;
            this.utf16TextLabel.Text = "UTF16文字列";
            // 
            // closeButton
            // 
            this.closeButton.Location = new System.Drawing.Point(507, 4);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(75, 23);
            this.closeButton.TabIndex = 4;
            this.closeButton.Text = "閉じる";
            this.closeButton.UseVisualStyleBackColor = true;
            this.closeButton.Click += new System.EventHandler(this.closeButton_Click);
            // 
            // utf16TextBox
            // 
            this.utf16TextBox.Location = new System.Drawing.Point(14, 245);
            this.utf16TextBox.Multiline = true;
            this.utf16TextBox.Name = "utf16TextBox";
            this.utf16TextBox.Size = new System.Drawing.Size(574, 210);
            this.utf16TextBox.TabIndex = 5;
            // 
            // jpTextBox
            // 
            this.jpTextBox.Location = new System.Drawing.Point(14, 33);
            this.jpTextBox.Multiline = true;
            this.jpTextBox.Name = "jpTextBox";
            this.jpTextBox.Size = new System.Drawing.Size(574, 177);
            this.jpTextBox.TabIndex = 6;
            // 
            // smallCharCheckBox
            // 
            this.smallCharCheckBox.AutoSize = true;
            this.smallCharCheckBox.Checked = true;
            this.smallCharCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.smallCharCheckBox.Location = new System.Drawing.Point(525, 220);
            this.smallCharCheckBox.Name = "smallCharCheckBox";
            this.smallCharCheckBox.Size = new System.Drawing.Size(60, 16);
            this.smallCharCheckBox.TabIndex = 7;
            this.smallCharCheckBox.Text = "小文字";
            this.smallCharCheckBox.UseVisualStyleBackColor = true;
            // 
            // UTF16ConverterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(596, 463);
            this.ControlBox = false;
            this.Controls.Add(this.smallCharCheckBox);
            this.Controls.Add(this.jpTextBox);
            this.Controls.Add(this.utf16TextBox);
            this.Controls.Add(this.closeButton);
            this.Controls.Add(this.utf16TextLabel);
            this.Controls.Add(this.convertTextLabel);
            this.Controls.Add(this.utf16UCheckBox);
            this.Controls.Add(this.toJpButton);
            this.Controls.Add(this.toUTF16Button);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "UTF16ConverterForm";
            this.ShowIcon = false;
            this.Text = "UTF16Converter";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button toUTF16Button;
        private System.Windows.Forms.Button toJpButton;
        private System.Windows.Forms.CheckBox utf16UCheckBox;
        private System.Windows.Forms.Label convertTextLabel;
        private System.Windows.Forms.Label utf16TextLabel;
        private System.Windows.Forms.Button closeButton;
        private System.Windows.Forms.TextBox utf16TextBox;
        private System.Windows.Forms.TextBox jpTextBox;
        private System.Windows.Forms.CheckBox smallCharCheckBox;
    }
}

