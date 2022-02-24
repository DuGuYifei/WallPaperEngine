
using System;
using System.Windows.Forms;

namespace UI
{
    partial class FormEngine
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormEngine));
            this.confirmButton = new System.Windows.Forms.Button();
            this.autoStartCheckBox = new System.Windows.Forms.CheckBox();
            this.browseButton = new System.Windows.Forms.Button();
            this.typeLabel = new System.Windows.Forms.Label();
            this.modeComboBox = new System.Windows.Forms.ComboBox();
            this.typeCombox = new System.Windows.Forms.ComboBox();
            this.webappCheckBox = new System.Windows.Forms.CheckBox();
            this.modeLabel = new System.Windows.Forms.Label();
            this.webappCommentLabel = new System.Windows.Forms.Label();
            this.pathTextBox = new System.Windows.Forms.TextBox();
            this.tutorialLinkLabel = new System.Windows.Forms.LinkLabel();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.退出ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openPanel = new System.Windows.Forms.Panel();
            this.screenLabel = new System.Windows.Forms.Label();
            this.screenCheckedListBox = new System.Windows.Forms.CheckedListBox();
            this.isCloseScreenCheckBox = new System.Windows.Forms.CheckBox();
            this.screenPanel = new System.Windows.Forms.Panel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.lastSettingLabel = new System.Windows.Forms.Label();
            this.lastSettingComboBox = new System.Windows.Forms.ComboBox();
            this.LanguageComboBox = new System.Windows.Forms.ComboBox();
            this.languageLabel = new System.Windows.Forms.Label();
            this.contextMenuStrip1.SuspendLayout();
            this.openPanel.SuspendLayout();
            this.screenPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // confirmButton
            // 
            this.confirmButton.Font = new System.Drawing.Font("幼圆", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.confirmButton.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.confirmButton.Location = new System.Drawing.Point(478, 401);
            this.confirmButton.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.confirmButton.Name = "confirmButton";
            this.confirmButton.Size = new System.Drawing.Size(47, 23);
            this.confirmButton.TabIndex = 6;
            this.confirmButton.Text = "确认";
            this.confirmButton.UseVisualStyleBackColor = true;
            this.confirmButton.Click += new System.EventHandler(this.confirmButton_Click);
            // 
            // autoStartCheckBox
            // 
            this.autoStartCheckBox.AutoSize = true;
            this.autoStartCheckBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.autoStartCheckBox.Location = new System.Drawing.Point(14, 324);
            this.autoStartCheckBox.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.autoStartCheckBox.Name = "autoStartCheckBox";
            this.autoStartCheckBox.Size = new System.Drawing.Size(527, 21);
            this.autoStartCheckBox.TabIndex = 9;
            this.autoStartCheckBox.Text = "开机自启动（开启后再取消不用担心会有残留注册表痕迹，详情见教程，[图片]不要勾选此项）";
            this.autoStartCheckBox.UseVisualStyleBackColor = true;
            // 
            // browseButton
            // 
            this.browseButton.Enabled = false;
            this.browseButton.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.browseButton.Location = new System.Drawing.Point(465, 42);
            this.browseButton.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(65, 23);
            this.browseButton.TabIndex = 11;
            this.browseButton.Text = "浏览";
            this.browseButton.UseVisualStyleBackColor = true;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // typeLabel
            // 
            this.typeLabel.AutoSize = true;
            this.typeLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.typeLabel.Location = new System.Drawing.Point(16, 8);
            this.typeLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.typeLabel.Name = "typeLabel";
            this.typeLabel.Size = new System.Drawing.Size(152, 17);
            this.typeLabel.TabIndex = 12;
            this.typeLabel.Text = "选择你要做壁纸的文件类型";
            // 
            // modeComboBox
            // 
            this.modeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modeComboBox.Items.AddRange(new object[] {
            "多屏相同壁纸",
            "多屏拆分壁纸"});
            this.modeComboBox.Location = new System.Drawing.Point(42, 102);
            this.modeComboBox.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.modeComboBox.Name = "modeComboBox";
            this.modeComboBox.Size = new System.Drawing.Size(96, 25);
            this.modeComboBox.TabIndex = 13;
            // 
            // typeCombox
            // 
            this.typeCombox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeCombox.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.typeCombox.Items.AddRange(new object[] {
            "视频/动态图",
            "图片",
            "程序",
            "html",
            "网址"});
            this.typeCombox.Location = new System.Drawing.Point(42, 40);
            this.typeCombox.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.typeCombox.Name = "typeCombox";
            this.typeCombox.Size = new System.Drawing.Size(96, 25);
            this.typeCombox.TabIndex = 14;
            this.typeCombox.SelectedIndexChanged += new System.EventHandler(this.typeCombox_SelectedIndexChanged);
            // 
            // webappCheckBox
            // 
            this.webappCheckBox.AutoSize = true;
            this.webappCheckBox.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.webappCheckBox.Location = new System.Drawing.Point(25, 134);
            this.webappCheckBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.webappCheckBox.Name = "webappCheckBox";
            this.webappCheckBox.Size = new System.Drawing.Size(143, 21);
            this.webappCheckBox.TabIndex = 23;
            this.webappCheckBox.Text = "chrome内核web程序";
            this.webappCheckBox.UseVisualStyleBackColor = true;
            // 
            // modeLabel
            // 
            this.modeLabel.AutoSize = true;
            this.modeLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.modeLabel.Location = new System.Drawing.Point(16, 76);
            this.modeLabel.Name = "modeLabel";
            this.modeLabel.Size = new System.Drawing.Size(303, 17);
            this.modeLabel.TabIndex = 25;
            this.modeLabel.Text = "选择壁纸的模式（只有1个显示器请选择“多屏拆分”）";
            // 
            // webappCommentLabel
            // 
            this.webappCommentLabel.AutoSize = true;
            this.webappCommentLabel.Location = new System.Drawing.Point(39, 159);
            this.webappCommentLabel.Name = "webappCommentLabel";
            this.webappCommentLabel.Size = new System.Drawing.Size(511, 17);
            this.webappCommentLabel.TabIndex = 26;
            this.webappCommentLabel.Text = "请在确认与壁纸无法交互后再选中此选项尝试。选择此选项会尽量寻找webapp正确的的窗口。";
            // 
            // pathTextBox
            // 
            this.pathTextBox.Enabled = false;
            this.pathTextBox.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.pathTextBox.Location = new System.Drawing.Point(158, 42);
            this.pathTextBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.pathTextBox.Name = "pathTextBox";
            this.pathTextBox.Size = new System.Drawing.Size(293, 23);
            this.pathTextBox.TabIndex = 27;
            // 
            // tutorialLinkLabel
            // 
            this.tutorialLinkLabel.ActiveLinkColor = System.Drawing.Color.Cyan;
            this.tutorialLinkLabel.AutoSize = true;
            this.tutorialLinkLabel.LinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.tutorialLinkLabel.Location = new System.Drawing.Point(173, 8);
            this.tutorialLinkLabel.Name = "tutorialLinkLabel";
            this.tutorialLinkLabel.Size = new System.Drawing.Size(357, 17);
            this.tutorialLinkLabel.TabIndex = 31;
            this.tutorialLinkLabel.TabStop = true;
            this.tutorialLinkLabel.Text = "建议先阅读：究极傻瓜简短无敌风火轮的备注和教程.txt(单击打开)";
            this.tutorialLinkLabel.VisitedLinkColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.tutorialLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.tutorialLinkLabel_LinkClicked);
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip1;
            this.notifyIcon1.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon1.Icon")));
            this.notifyIcon1.Text = "Wallpaper Engine";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon1_MouseDoubleClick);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.退出ToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(101, 26);
            // 
            // 退出ToolStripMenuItem
            // 
            this.退出ToolStripMenuItem.Name = "退出ToolStripMenuItem";
            this.退出ToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.退出ToolStripMenuItem.Text = "退出";
            this.退出ToolStripMenuItem.Click += new System.EventHandler(this.退出ToolStripMenuItem_Click);
            // 
            // openPanel
            // 
            this.openPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.openPanel.Controls.Add(this.tutorialLinkLabel);
            this.openPanel.Controls.Add(this.typeLabel);
            this.openPanel.Controls.Add(this.pathTextBox);
            this.openPanel.Controls.Add(this.browseButton);
            this.openPanel.Controls.Add(this.modeLabel);
            this.openPanel.Controls.Add(this.typeCombox);
            this.openPanel.Controls.Add(this.modeComboBox);
            this.openPanel.Controls.Add(this.webappCommentLabel);
            this.openPanel.Controls.Add(this.webappCheckBox);
            this.openPanel.Location = new System.Drawing.Point(-6, 0);
            this.openPanel.Name = "openPanel";
            this.openPanel.Size = new System.Drawing.Size(558, 193);
            this.openPanel.TabIndex = 32;
            // 
            // screenLabel
            // 
            this.screenLabel.AutoSize = true;
            this.screenLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.screenLabel.Location = new System.Drawing.Point(17, 14);
            this.screenLabel.Name = "screenLabel";
            this.screenLabel.Size = new System.Drawing.Size(208, 17);
            this.screenLabel.TabIndex = 29;
            this.screenLabel.Text = "选择你的屏幕(按显示设置标识顺序)：";
            // 
            // screenCheckedListBox
            // 
            this.screenCheckedListBox.CheckOnClick = true;
            this.screenCheckedListBox.FormattingEnabled = true;
            this.screenCheckedListBox.Items.AddRange(new object[] {
            "Screen1"});
            this.screenCheckedListBox.Location = new System.Drawing.Point(253, 14);
            this.screenCheckedListBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.screenCheckedListBox.Name = "screenCheckedListBox";
            this.screenCheckedListBox.ScrollAlwaysVisible = true;
            this.screenCheckedListBox.Size = new System.Drawing.Size(135, 58);
            this.screenCheckedListBox.TabIndex = 30;
            // 
            // isCloseScreenCheckBox
            // 
            this.isCloseScreenCheckBox.AutoSize = true;
            this.isCloseScreenCheckBox.Location = new System.Drawing.Point(25, 78);
            this.isCloseScreenCheckBox.Name = "isCloseScreenCheckBox";
            this.isCloseScreenCheckBox.Size = new System.Drawing.Size(135, 21);
            this.isCloseScreenCheckBox.TabIndex = 31;
            this.isCloseScreenCheckBox.Text = "关闭选中屏幕的壁纸";
            this.isCloseScreenCheckBox.UseVisualStyleBackColor = true;
            this.isCloseScreenCheckBox.CheckedChanged += new System.EventHandler(this.isCloseScreenCheckBox_CheckedChanged);
            // 
            // screenPanel
            // 
            this.screenPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.screenPanel.Controls.Add(this.isCloseScreenCheckBox);
            this.screenPanel.Controls.Add(this.screenCheckedListBox);
            this.screenPanel.Controls.Add(this.screenLabel);
            this.screenPanel.Location = new System.Drawing.Point(-6, 189);
            this.screenPanel.Name = "screenPanel";
            this.screenPanel.Size = new System.Drawing.Size(558, 119);
            this.screenPanel.TabIndex = 33;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // lastSettingLabel
            // 
            this.lastSettingLabel.AutoSize = true;
            this.lastSettingLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(19)))), ((int)(((byte)(148)))), ((int)(((byte)(219)))));
            this.lastSettingLabel.Location = new System.Drawing.Point(8, 364);
            this.lastSettingLabel.Name = "lastSettingLabel";
            this.lastSettingLabel.Size = new System.Drawing.Size(248, 17);
            this.lastSettingLabel.TabIndex = 34;
            this.lastSettingLabel.Text = "下次启动时希望按照最后的多少次设置启动？";
            // 
            // lastSettingComboBox
            // 
            this.lastSettingComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.lastSettingComboBox.Items.AddRange(new object[] {
            "0",
            "1",
            "2",
            "3",
            "4"});
            this.lastSettingComboBox.Location = new System.Drawing.Point(262, 361);
            this.lastSettingComboBox.Name = "lastSettingComboBox";
            this.lastSettingComboBox.Size = new System.Drawing.Size(34, 25);
            this.lastSettingComboBox.TabIndex = 35;
            this.lastSettingComboBox.SelectedIndex = 1;
            // 
            // LanguageComboBox
            // 
            this.LanguageComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.LanguageComboBox.Font = new System.Drawing.Font("微软雅黑", 6.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.LanguageComboBox.Items.AddRange(new object[] {
            "简体中文",
            "English"});
            this.LanguageComboBox.Location = new System.Drawing.Point(98, 401);
            this.LanguageComboBox.Name = "LanguageComboBox";
            this.LanguageComboBox.Size = new System.Drawing.Size(65, 22);
            this.LanguageComboBox.TabIndex = 36;
            this.LanguageComboBox.SelectedIndexChanged += new System.EventHandler(this.LanguageComboBox_SelectedIndexChanged);
            // 
            // languageLabel
            // 
            this.languageLabel.AutoSize = true;
            this.languageLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(241)))), ((int)(((byte)(241)))), ((int)(((byte)(241)))));
            this.languageLabel.Location = new System.Drawing.Point(27, 402);
            this.languageLabel.Name = "languageLabel";
            this.languageLabel.Size = new System.Drawing.Size(65, 17);
            this.languageLabel.TabIndex = 37;
            this.languageLabel.Text = "Language";
            // 
            // FormEngine
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.ClientSize = new System.Drawing.Size(546, 436);
            this.Controls.Add(this.languageLabel);
            this.Controls.Add(this.LanguageComboBox);
            this.Controls.Add(this.lastSettingComboBox);
            this.Controls.Add(this.lastSettingLabel);
            this.Controls.Add(this.autoStartCheckBox);
            this.Controls.Add(this.confirmButton);
            this.Controls.Add(this.openPanel);
            this.Controls.Add(this.screenPanel);
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(241)))), ((int)(((byte)(241)))), ((int)(((byte)(241)))));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2, 3, 2, 3);
            this.MaximizeBox = false;
            this.Name = "FormEngine";
            this.Text = "Wallpaper Engine";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.contextMenuStrip1.ResumeLayout(false);
            this.openPanel.ResumeLayout(false);
            this.openPanel.PerformLayout();
            this.screenPanel.ResumeLayout(false);
            this.screenPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button confirmButton;
        private System.Windows.Forms.CheckBox autoStartCheckBox;
        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.Label typeLabel;
        private System.Windows.Forms.ComboBox modeComboBox;
        private System.Windows.Forms.ComboBox typeCombox;
        private System.Windows.Forms.CheckBox webappCheckBox;
        private System.Windows.Forms.Label modeLabel;
        private System.Windows.Forms.Label webappCommentLabel;
        private System.Windows.Forms.TextBox pathTextBox;
        private System.Windows.Forms.LinkLabel tutorialLinkLabel;
        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private ContextMenuStrip contextMenuStrip1;
        private ToolStripMenuItem 退出ToolStripMenuItem;
        private Panel openPanel;
        private Label screenLabel;
        private CheckedListBox screenCheckedListBox;
        private CheckBox isCloseScreenCheckBox;
        private Panel screenPanel;
        private OpenFileDialog openFileDialog1;
        private Label lastSettingLabel;
        private ComboBox lastSettingComboBox;
        private ComboBox LanguageComboBox;
        private Label languageLabel;
    }
}

