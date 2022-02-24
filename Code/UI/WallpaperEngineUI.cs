using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace UI
{
    public partial class FormEngine : Form
    {
        public FormEngine()
        {
            InitializeComponent();
            int screenNum = Screen.AllScreens.Count();
            int i = 2;
            while (i <= screenNum)
            {
                this.screenCheckedListBox.Items.Add("Screen" + i);
                i++;
            }
            
            System.Diagnostics.Process.Start("WallpaperEngine.exe", "a");
        }

        private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (this.WindowState == FormWindowState.Minimized)
            {
                this.WindowState = FormWindowState.Normal;
                this.ShowInTaskbar = true;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            //取消关闭窗口
            e.Cancel = true;
            //最小化主窗口
            this.WindowState = FormWindowState.Minimized;
            //不在系统任务栏显示主窗口图标
            this.ShowInTaskbar = false;
            //提示气泡
            this.notifyIcon1.ShowBalloonTip(3000, "最小化到托盘", "程序已经缩小到托盘，双击打开程序。", ToolTipIcon.Info);
        }

        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //this.notifyIcon1.Visible = false;

            System.Diagnostics.Process.Start("WallpaperEngine.exe", "q");
            
            this.Dispose(true);
            this.Close();
        }

        private void tutorialLinkLabel_LinkClicked(object sender, EventArgs e)
        {
            //打开文件
            System.Diagnostics.Process.Start("究极傻瓜简短无敌风火轮的备注和教程.txt");
        }

        private void confirmButton_Click(object sender, EventArgs e)
        {
            //先判断是否有东西没填
            //首先如果是关壁纸
            if(this.isCloseScreenCheckBox.Checked)
            {
                if (this.screenCheckedListBox.CheckedIndices.Count < 1)
                {
                    MessageBox.Show("请选屏幕！");
                    return;
                }
            }
            //再看打开壁纸时
            else
            {
                if(this.pathTextBox.Text == "")
                {
                    MessageBox.Show("没有路径或者网址");
                    return;
                }
                else if(this.modeComboBox.Text == "")
                {
                    MessageBox.Show("没有选择模式");
                    return;
                }
                else if(this.screenCheckedListBox.CheckedItems.Count < 1)
                {
                    MessageBox.Show("请选择屏幕");
                    return;
                }
            }

            if (File.Exists("./resource/UIContent3.wpeg"))
            {
                File.Delete("./resource/UIContent4.wpeg");
                File.Copy("./resource/UIContent3.wpeg", "./resource/UIContent4.wpeg");
            }
            if (File.Exists("./resource/UIContent2.wpeg"))
            {
                File.Delete("./resource/UIContent3.wpeg");
                File.Copy("./resource/UIContent2.wpeg", "./resource/UIContent3.wpeg");
            }
            if (File.Exists("./resource/UIContent1.wpeg"))
            {
                File.Delete("./resource/UIContent2.wpeg");
                File.Copy("./resource/UIContent1.wpeg", "./resource/UIContent2.wpeg");
            }

            using (StreamWriter sw = new StreamWriter("./resource/UIContent1.wpeg"))
            {
                sw.WriteLine(this.typeCombox.SelectedIndex);                    //1.文件类型
                sw.WriteLine(this.pathTextBox.Text);                            //2.文件完整路径
                sw.WriteLine(this.modeComboBox.SelectedIndex);                  //3.屏幕模式
                sw.WriteLine(Convert.ToInt32(this.webappCheckBox.Checked));     //4.是否是webapp
                sw.Write(this.screenCheckedListBox.CheckedItems.Count + " ");   //5.1选中的屏幕数量
                foreach(int item in this.screenCheckedListBox.CheckedIndices)   //5.2选中的屏幕序号
                    sw.Write(item + " ");
                sw.WriteLine();
                sw.WriteLine(Convert.ToInt32(this.autoStartCheckBox.Checked));  //6.是否开机自启动
                sw.WriteLine(Convert.ToInt32(this.isCloseScreenCheckBox.Checked));//7.是否是关闭壁纸命令
                sw.WriteLine(this.lastSettingComboBox.SelectedIndex);           //8.按最后的几次设置自动打开
            }
            this.confirmButton.Enabled = false;
            MessageBox.Show("接收成功，请等待几秒，如果屏幕没有变化：\n1.你乱来选错文件；\n2.你电脑太慢继续等；\n3.程序员的程序遇到问题80%都能靠重启解决");

            System.Diagnostics.Process.Start("WallpaperEngine.exe", "s");
            
            this.confirmButton.Enabled = true;
        }

        private void typeCombox_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.pathTextBox.Text = "";
            if (this.typeCombox.SelectedIndex == 4)
            {
                this.pathTextBox.Enabled = true;
                this.browseButton.Enabled = false;
            }
            else
            {
                this.pathTextBox.Enabled = false;
                this.browseButton.Enabled = true;
                int type = this.typeCombox.SelectedIndex;
                if (type == 0)
                {
                    this.openFileDialog1.Filter = "视频或图片(格式太多就不定义了^v^)|*.*";
                }
                else if(type == 1)
                {
                    this.screenCheckedListBox.Enabled = false;
                    this.modeComboBox.SelectedIndex = 0;
                    this.modeComboBox.Enabled = false;
                    for(int i = 0;i< this.screenCheckedListBox.Items.Count;i++)
                    {
                        this.screenCheckedListBox.SetItemChecked(i, true);
                    }
                    this.openFileDialog1.Filter = "图片|*.jpg;*.jpeg;*.bmp;*.dib;*.png;*.jfif;*.jpe;*.gif;*.tif;*.tiff;*.wdp";
                }
                else if(type == 2)
                {
                    this.openFileDialog1.Filter = "程序|*.exe";
                }
                else if(type == 3)
                {
                    this.openFileDialog1.Filter = "超文本(html)|*.html";
                }
                if (type != 1)
                {
                    this.screenCheckedListBox.Enabled = true;
                    this.modeComboBox.Enabled = true;
                }
            }
        }

        private void isCloseScreenCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (this.isCloseScreenCheckBox.Checked)
            {
                this.openPanel.Enabled = false;
            }
            else
                this.openPanel.Enabled = true;
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            this.openFileDialog1.ShowDialog();
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            this.pathTextBox.Text = this.openFileDialog1.FileName;
        }

        private void LanguageComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(this.LanguageComboBox.SelectedIndex == 0)
            {
                // 
                // confirmButton
                // 
                this.confirmButton.Text = "确认";
                // 
                // autoStartCheckBox
                // 
                this.autoStartCheckBox.Text = "开机自启动（开启后再取消不用担心会有残留注册表痕迹，详情见教程，[图片]不要勾选此项）";
                // 
                // browseButton
                // 
                this.browseButton.Text = "浏览";
                // 
                // typeLabel
                // 
                this.typeLabel.Text = "选择你要做壁纸的文件类型";
                // 
                // modeComboBox
                // 
                this.modeComboBox.Items.Clear();
                this.modeComboBox.Items.AddRange(new object[] {
            "多屏相同壁纸",
            "多屏拆分壁纸"});
                // 
                // typeCombox
                // 
                this.typeCombox.Items.Clear();
                this.typeCombox.Items.AddRange(new object[] {
            "视频/动态图",
            "图片",
            "程序",
            "html",
            "网址"});
                // 
                // webappCheckBox
                // 
                this.webappCheckBox.Text = "chrome内核web程序";
                // 
                // modeLabel
                // 
                this.modeLabel.Text = "选择壁纸的模式（只有1个显示器请选择“多屏拆分”）";
                // 
                // webappCommentLabel
                // 
                this.webappCommentLabel.Text = "请在确认与壁纸无法交互后再选中此选项尝试。选择此选项会尽量寻找webapp正确的的窗口。";
                // 
                // tutorialLinkLabel
                // 
                this.tutorialLinkLabel.Text = "建议先阅读：究极傻瓜简短无敌风火轮的备注和教程.txt(单击打开)";
                // 
                // 退出ToolStripMenuItem
                // 
                this.退出ToolStripMenuItem.Text = "退出";
                // 
                // screenLabel
                // 
                this.screenLabel.Text = "选择你的屏幕(按显示设置标识顺序)：";
                // 
                // isCloseScreenCheckBox
                // 
                this.isCloseScreenCheckBox.Text = "关闭选中屏幕的壁纸";
                // 
                // lastSettingLabel
                // 
                this.lastSettingLabel.Text = "下次启动时希望按照最后的多少次设置启动？";
                this.lastSettingComboBox.Location = new Point(this.lastSettingComboBox.Location.X, 361);
            }
            if (this.LanguageComboBox.SelectedIndex == 1)
            {
                // 
                // confirmButton
                // 
                this.confirmButton.Text = "Init";
                // 
                // autoStartCheckBox
                // 
                this.autoStartCheckBox.Text = "Self-start\n(picture type don't need choose.When cancel,it will clean traces in your computer)";
                // 
                // browseButton
                // 
                this.browseButton.Text = "Browse";
                // 
                // typeLabel
                // 
                this.typeLabel.Text = "Choose wallpaper type";
                // 
                // modeComboBox
                // 
                this.modeComboBox.Items.Clear();
                this.modeComboBox.Items.AddRange(new object[] {
            "same in screens",
            "split to screens"});
                // 
                // typeCombox
                // 
                this.typeCombox.Items.Clear();
                this.typeCombox.Items.AddRange(new object[] {
            "video/dynamic graph",
            "picture",
            "exe",
            "html",
            "website"});
                // 
                // webappCheckBox
                // 
                this.webappCheckBox.Text = "chrome core web app";
                // 
                // modeLabel
                // 
                this.modeLabel.Text = "Choose mode of wall paper（Choose \"split\" when only have one screen)";
                // 
                // webappCommentLabel
                // 
                this.webappCommentLabel.Text = "Only click after cannot interact with wallpaper.It will try to find correct window.";
                // 
                // tutorialLinkLabel
                // 
                this.tutorialLinkLabel.Text = "Advice read：tutorial.txt(Click to open)";
                // 
                // 退出ToolStripMenuItem
                // 
                this.退出ToolStripMenuItem.Text = "Quit";
                // 
                // screenLabel
                // 
                this.screenLabel.Text = "Choose screen\n(sequence as computer setting)";
                // 
                // screenCheckedListBox
                // 

                // 
                // isCloseScreenCheckBox
                // 
                this.isCloseScreenCheckBox.Text = "Close wallpaper of chosen screen";
                // 
                // lastSettingLabel
                // 
                this.lastSettingLabel.Text = "How many times of setting recently\nwill auto start next time?";
                this.lastSettingComboBox.Location = new Point (this.lastSettingComboBox.Location.X, 370);
            }
        }
    }
}
