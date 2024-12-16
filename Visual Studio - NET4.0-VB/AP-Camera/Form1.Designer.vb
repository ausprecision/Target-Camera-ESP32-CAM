<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.timer_wifi = New System.Windows.Forms.Timer(Me.components)
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.timer_fps = New System.Windows.Forms.Timer(Me.components)
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Button2 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_1 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_2 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_3 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_4 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_5 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_6 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_7 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_8 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_9 = New System.Windows.Forms.Button()
        Me.btn_wifi_pwr_10 = New System.Windows.Forms.Button()
        Me.btn_res1 = New System.Windows.Forms.Button()
        Me.btn_res2 = New System.Windows.Forms.Button()
        Me.btn_res3 = New System.Windows.Forms.Button()
        Me.btn_res4 = New System.Windows.Forms.Button()
        Me.btn_res5 = New System.Windows.Forms.Button()
        Me.btn_res6 = New System.Windows.Forms.Button()
        Me.btn_hflip = New System.Windows.Forms.Button()
        Me.btn_vflip = New System.Windows.Forms.Button()
        Me.btn_cpu240 = New System.Windows.Forms.Button()
        Me.btn_cpu80 = New System.Windows.Forms.Button()
        Me.btn_cpu160 = New System.Windows.Forms.Button()
        Me.btn_aec2 = New System.Windows.Forms.Button()
        Me.updown_jpeg = New System.Windows.Forms.NumericUpDown()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.updown_xclk = New System.Windows.Forms.NumericUpDown()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.updown_wifichan = New System.Windows.Forms.NumericUpDown()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.textbox_ssid = New System.Windows.Forms.TextBox()
        Me.textbox_pass = New System.Windows.Forms.TextBox()
        Me.btn_reboot = New System.Windows.Forms.Button()
        Me.timer_UDP_Connected = New System.Windows.Forms.Timer(Me.components)
        Me.Button3 = New System.Windows.Forms.Button()
        Me.Button4 = New System.Windows.Forms.Button()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.updown_jpeg, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.updown_xclk, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.updown_wifichan, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Button1
        '
        Me.Button1.Location = New System.Drawing.Point(12, 61)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(143, 23)
        Me.Button1.TabIndex = 0
        Me.Button1.Text = "Start Stream"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(9, 25)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(146, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "[WiFi] Initializing..."
        '
        'Label2
        '
        Me.Label2.Location = New System.Drawing.Point(9, 9)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(982, 13)
        Me.Label2.TabIndex = 2
        Me.Label2.Text = "[Camera] Initializing..."
        '
        'timer_wifi
        '
        Me.timer_wifi.Enabled = True
        Me.timer_wifi.Interval = 10000
        '
        'PictureBox1
        '
        Me.PictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.PictureBox1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.PictureBox1.Location = New System.Drawing.Point(0, 0)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(984, 596)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBox1.TabIndex = 3
        Me.PictureBox1.TabStop = False
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(161, 25)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(130, 13)
        Me.Label3.TabIndex = 4
        Me.Label3.Text = "[Resolution] Initializing..."
        '
        'timer_fps
        '
        Me.timer_fps.Interval = 10000
        '
        'Label4
        '
        Me.Label4.Location = New System.Drawing.Point(297, 25)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(104, 13)
        Me.Label4.TabIndex = 5
        Me.Label4.Text = "[FPS] Initializing..."
        '
        'Button2
        '
        Me.Button2.Location = New System.Drawing.Point(12, 90)
        Me.Button2.Name = "Button2"
        Me.Button2.Size = New System.Drawing.Size(143, 23)
        Me.Button2.TabIndex = 6
        Me.Button2.Text = "Save Image"
        Me.Button2.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_1
        '
        Me.btn_wifi_pwr_1.Enabled = False
        Me.btn_wifi_pwr_1.Location = New System.Drawing.Point(12, 148)
        Me.btn_wifi_pwr_1.Name = "btn_wifi_pwr_1"
        Me.btn_wifi_pwr_1.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_1.TabIndex = 0
        Me.btn_wifi_pwr_1.Text = "WiFi 5 dBm (25%)"
        Me.btn_wifi_pwr_1.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_2
        '
        Me.btn_wifi_pwr_2.Enabled = False
        Me.btn_wifi_pwr_2.Location = New System.Drawing.Point(12, 177)
        Me.btn_wifi_pwr_2.Name = "btn_wifi_pwr_2"
        Me.btn_wifi_pwr_2.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_2.TabIndex = 0
        Me.btn_wifi_pwr_2.Text = "WiFi 7 dBm (35%)"
        Me.btn_wifi_pwr_2.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_3
        '
        Me.btn_wifi_pwr_3.Enabled = False
        Me.btn_wifi_pwr_3.Location = New System.Drawing.Point(12, 206)
        Me.btn_wifi_pwr_3.Name = "btn_wifi_pwr_3"
        Me.btn_wifi_pwr_3.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_3.TabIndex = 0
        Me.btn_wifi_pwr_3.Text = "WiFi 8.5 dBm (45%)"
        Me.btn_wifi_pwr_3.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_4
        '
        Me.btn_wifi_pwr_4.Enabled = False
        Me.btn_wifi_pwr_4.Location = New System.Drawing.Point(12, 235)
        Me.btn_wifi_pwr_4.Name = "btn_wifi_pwr_4"
        Me.btn_wifi_pwr_4.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_4.TabIndex = 0
        Me.btn_wifi_pwr_4.Text = "WiFi 11.0 dBm (55%)"
        Me.btn_wifi_pwr_4.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_5
        '
        Me.btn_wifi_pwr_5.Enabled = False
        Me.btn_wifi_pwr_5.Location = New System.Drawing.Point(12, 264)
        Me.btn_wifi_pwr_5.Name = "btn_wifi_pwr_5"
        Me.btn_wifi_pwr_5.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_5.TabIndex = 0
        Me.btn_wifi_pwr_5.Text = "WiFi 13.0 dBm (65%)"
        Me.btn_wifi_pwr_5.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_6
        '
        Me.btn_wifi_pwr_6.Enabled = False
        Me.btn_wifi_pwr_6.Location = New System.Drawing.Point(12, 293)
        Me.btn_wifi_pwr_6.Name = "btn_wifi_pwr_6"
        Me.btn_wifi_pwr_6.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_6.TabIndex = 0
        Me.btn_wifi_pwr_6.Text = "WiFi 15.0 dBm (75%)"
        Me.btn_wifi_pwr_6.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_7
        '
        Me.btn_wifi_pwr_7.Enabled = False
        Me.btn_wifi_pwr_7.Location = New System.Drawing.Point(12, 322)
        Me.btn_wifi_pwr_7.Name = "btn_wifi_pwr_7"
        Me.btn_wifi_pwr_7.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_7.TabIndex = 0
        Me.btn_wifi_pwr_7.Text = "WiFi 17.0 dBm (85%)"
        Me.btn_wifi_pwr_7.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_8
        '
        Me.btn_wifi_pwr_8.Enabled = False
        Me.btn_wifi_pwr_8.Location = New System.Drawing.Point(12, 351)
        Me.btn_wifi_pwr_8.Name = "btn_wifi_pwr_8"
        Me.btn_wifi_pwr_8.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_8.TabIndex = 0
        Me.btn_wifi_pwr_8.Text = "WiFi 18.5 dBm (90%)"
        Me.btn_wifi_pwr_8.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_9
        '
        Me.btn_wifi_pwr_9.Enabled = False
        Me.btn_wifi_pwr_9.Location = New System.Drawing.Point(12, 380)
        Me.btn_wifi_pwr_9.Name = "btn_wifi_pwr_9"
        Me.btn_wifi_pwr_9.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_9.TabIndex = 0
        Me.btn_wifi_pwr_9.Text = "WiFi 19.0 dBm (95%)"
        Me.btn_wifi_pwr_9.UseVisualStyleBackColor = True
        '
        'btn_wifi_pwr_10
        '
        Me.btn_wifi_pwr_10.Enabled = False
        Me.btn_wifi_pwr_10.Location = New System.Drawing.Point(12, 409)
        Me.btn_wifi_pwr_10.Name = "btn_wifi_pwr_10"
        Me.btn_wifi_pwr_10.Size = New System.Drawing.Size(143, 23)
        Me.btn_wifi_pwr_10.TabIndex = 0
        Me.btn_wifi_pwr_10.TabStop = False
        Me.btn_wifi_pwr_10.Text = "WiFi 19.5 dBm (100%)"
        Me.btn_wifi_pwr_10.UseVisualStyleBackColor = True
        '
        'btn_res1
        '
        Me.btn_res1.Enabled = False
        Me.btn_res1.Location = New System.Drawing.Point(846, 61)
        Me.btn_res1.Name = "btn_res1"
        Me.btn_res1.Size = New System.Drawing.Size(126, 23)
        Me.btn_res1.TabIndex = 0
        Me.btn_res1.Text = "VGA (640x480)"
        Me.btn_res1.UseVisualStyleBackColor = True
        '
        'btn_res2
        '
        Me.btn_res2.Enabled = False
        Me.btn_res2.Location = New System.Drawing.Point(846, 90)
        Me.btn_res2.Name = "btn_res2"
        Me.btn_res2.Size = New System.Drawing.Size(126, 23)
        Me.btn_res2.TabIndex = 0
        Me.btn_res2.Text = "SVGA (800x600)"
        Me.btn_res2.UseVisualStyleBackColor = True
        '
        'btn_res3
        '
        Me.btn_res3.Enabled = False
        Me.btn_res3.Location = New System.Drawing.Point(846, 119)
        Me.btn_res3.Name = "btn_res3"
        Me.btn_res3.Size = New System.Drawing.Size(126, 23)
        Me.btn_res3.TabIndex = 0
        Me.btn_res3.Text = "XGA (1024x768)"
        Me.btn_res3.UseVisualStyleBackColor = True
        '
        'btn_res4
        '
        Me.btn_res4.Enabled = False
        Me.btn_res4.Location = New System.Drawing.Point(846, 148)
        Me.btn_res4.Name = "btn_res4"
        Me.btn_res4.Size = New System.Drawing.Size(126, 23)
        Me.btn_res4.TabIndex = 0
        Me.btn_res4.Text = "HD (1280x720)"
        Me.btn_res4.UseVisualStyleBackColor = True
        '
        'btn_res5
        '
        Me.btn_res5.Enabled = False
        Me.btn_res5.Location = New System.Drawing.Point(846, 177)
        Me.btn_res5.Name = "btn_res5"
        Me.btn_res5.Size = New System.Drawing.Size(126, 23)
        Me.btn_res5.TabIndex = 0
        Me.btn_res5.Text = "SXGA (1280x1024)"
        Me.btn_res5.UseVisualStyleBackColor = True
        '
        'btn_res6
        '
        Me.btn_res6.Enabled = False
        Me.btn_res6.Location = New System.Drawing.Point(846, 206)
        Me.btn_res6.Name = "btn_res6"
        Me.btn_res6.Size = New System.Drawing.Size(126, 23)
        Me.btn_res6.TabIndex = 0
        Me.btn_res6.Text = "UXGA (1600x1200)"
        Me.btn_res6.UseVisualStyleBackColor = True
        '
        'btn_hflip
        '
        Me.btn_hflip.Enabled = False
        Me.btn_hflip.Location = New System.Drawing.Point(846, 287)
        Me.btn_hflip.Name = "btn_hflip"
        Me.btn_hflip.Size = New System.Drawing.Size(126, 23)
        Me.btn_hflip.TabIndex = 0
        Me.btn_hflip.Text = "Horizontal Flip"
        Me.btn_hflip.UseVisualStyleBackColor = True
        '
        'btn_vflip
        '
        Me.btn_vflip.Enabled = False
        Me.btn_vflip.Location = New System.Drawing.Point(846, 316)
        Me.btn_vflip.Name = "btn_vflip"
        Me.btn_vflip.Size = New System.Drawing.Size(126, 23)
        Me.btn_vflip.TabIndex = 0
        Me.btn_vflip.Text = "Vertical Flip"
        Me.btn_vflip.UseVisualStyleBackColor = True
        '
        'btn_cpu240
        '
        Me.btn_cpu240.Enabled = False
        Me.btn_cpu240.Location = New System.Drawing.Point(846, 393)
        Me.btn_cpu240.Name = "btn_cpu240"
        Me.btn_cpu240.Size = New System.Drawing.Size(126, 23)
        Me.btn_cpu240.TabIndex = 0
        Me.btn_cpu240.Text = "240Mhz"
        Me.btn_cpu240.UseVisualStyleBackColor = True
        '
        'btn_cpu80
        '
        Me.btn_cpu80.Enabled = False
        Me.btn_cpu80.Location = New System.Drawing.Point(846, 451)
        Me.btn_cpu80.Name = "btn_cpu80"
        Me.btn_cpu80.Size = New System.Drawing.Size(126, 23)
        Me.btn_cpu80.TabIndex = 0
        Me.btn_cpu80.Text = "80Mhz"
        Me.btn_cpu80.UseVisualStyleBackColor = True
        '
        'btn_cpu160
        '
        Me.btn_cpu160.Enabled = False
        Me.btn_cpu160.Location = New System.Drawing.Point(846, 422)
        Me.btn_cpu160.Name = "btn_cpu160"
        Me.btn_cpu160.Size = New System.Drawing.Size(126, 23)
        Me.btn_cpu160.TabIndex = 0
        Me.btn_cpu160.Text = "160Mhz"
        Me.btn_cpu160.UseVisualStyleBackColor = True
        '
        'btn_aec2
        '
        Me.btn_aec2.Enabled = False
        Me.btn_aec2.Location = New System.Drawing.Point(846, 345)
        Me.btn_aec2.Name = "btn_aec2"
        Me.btn_aec2.Size = New System.Drawing.Size(126, 23)
        Me.btn_aec2.TabIndex = 0
        Me.btn_aec2.Text = "Auto Exposure"
        Me.btn_aec2.UseVisualStyleBackColor = True
        '
        'updown_jpeg
        '
        Me.updown_jpeg.Enabled = False
        Me.updown_jpeg.Location = New System.Drawing.Point(931, 235)
        Me.updown_jpeg.Maximum = New Decimal(New Integer() {63, 0, 0, 0})
        Me.updown_jpeg.Minimum = New Decimal(New Integer() {10, 0, 0, 0})
        Me.updown_jpeg.Name = "updown_jpeg"
        Me.updown_jpeg.Size = New System.Drawing.Size(41, 20)
        Me.updown_jpeg.TabIndex = 32
        Me.updown_jpeg.Value = New Decimal(New Integer() {10, 0, 0, 0})
        '
        'Label5
        '
        Me.Label5.Location = New System.Drawing.Point(860, 239)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(65, 13)
        Me.Label5.TabIndex = 33
        Me.Label5.Text = "JPEG Qual:"
        Me.Label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'Label6
        '
        Me.Label6.Location = New System.Drawing.Point(860, 264)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(65, 13)
        Me.Label6.TabIndex = 34
        Me.Label6.Text = "XCLK Mhz:"
        Me.Label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'updown_xclk
        '
        Me.updown_xclk.Enabled = False
        Me.updown_xclk.Increment = New Decimal(New Integer() {2, 0, 0, 0})
        Me.updown_xclk.Location = New System.Drawing.Point(931, 261)
        Me.updown_xclk.Maximum = New Decimal(New Integer() {20, 0, 0, 0})
        Me.updown_xclk.Minimum = New Decimal(New Integer() {8, 0, 0, 0})
        Me.updown_xclk.Name = "updown_xclk"
        Me.updown_xclk.Size = New System.Drawing.Size(41, 20)
        Me.updown_xclk.TabIndex = 35
        Me.updown_xclk.Value = New Decimal(New Integer() {8, 0, 0, 0})
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Location = New System.Drawing.Point(843, 380)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(102, 13)
        Me.Label7.TabIndex = 36
        Me.Label7.Text = "ESP32 CPU Speed:"
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Location = New System.Drawing.Point(843, 45)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(99, 13)
        Me.Label8.TabIndex = 37
        Me.Label8.Text = "Camera Resolution:"
        '
        'Label9
        '
        Me.Label9.Location = New System.Drawing.Point(12, 480)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(64, 20)
        Me.Label9.TabIndex = 39
        Me.Label9.Text = "WiFi CHAN:"
        Me.Label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'updown_wifichan
        '
        Me.updown_wifichan.Enabled = False
        Me.updown_wifichan.Location = New System.Drawing.Point(82, 480)
        Me.updown_wifichan.Maximum = New Decimal(New Integer() {11, 0, 0, 0})
        Me.updown_wifichan.Minimum = New Decimal(New Integer() {1, 0, 0, 0})
        Me.updown_wifichan.Name = "updown_wifichan"
        Me.updown_wifichan.Size = New System.Drawing.Size(73, 20)
        Me.updown_wifichan.TabIndex = 38
        Me.updown_wifichan.Value = New Decimal(New Integer() {10, 0, 0, 0})
        '
        'Label10
        '
        Me.Label10.Location = New System.Drawing.Point(12, 506)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(64, 20)
        Me.Label10.TabIndex = 40
        Me.Label10.Text = "WiFi SSID:"
        Me.Label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'Label11
        '
        Me.Label11.Location = New System.Drawing.Point(12, 532)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(64, 20)
        Me.Label11.TabIndex = 41
        Me.Label11.Text = "WiFi PASS:"
        Me.Label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight
        '
        'textbox_ssid
        '
        Me.textbox_ssid.Enabled = False
        Me.textbox_ssid.Location = New System.Drawing.Point(82, 506)
        Me.textbox_ssid.Name = "textbox_ssid"
        Me.textbox_ssid.Size = New System.Drawing.Size(73, 20)
        Me.textbox_ssid.TabIndex = 42
        Me.textbox_ssid.Text = "apcamera"
        '
        'textbox_pass
        '
        Me.textbox_pass.Enabled = False
        Me.textbox_pass.Location = New System.Drawing.Point(82, 532)
        Me.textbox_pass.Name = "textbox_pass"
        Me.textbox_pass.Size = New System.Drawing.Size(73, 20)
        Me.textbox_pass.TabIndex = 43
        Me.textbox_pass.Text = "tspass2000"
        '
        'btn_reboot
        '
        Me.btn_reboot.Location = New System.Drawing.Point(846, 558)
        Me.btn_reboot.Name = "btn_reboot"
        Me.btn_reboot.Size = New System.Drawing.Size(126, 23)
        Me.btn_reboot.TabIndex = 0
        Me.btn_reboot.Text = "Reboot Camera"
        Me.btn_reboot.UseVisualStyleBackColor = True
        '
        'timer_UDP_Connected
        '
        Me.timer_UDP_Connected.Enabled = True
        Me.timer_UDP_Connected.Interval = 1000
        '
        'Button3
        '
        Me.Button3.Enabled = False
        Me.Button3.Location = New System.Drawing.Point(12, 451)
        Me.Button3.Name = "Button3"
        Me.Button3.Size = New System.Drawing.Size(143, 23)
        Me.Button3.TabIndex = 44
        Me.Button3.TabStop = False
        Me.Button3.Text = "Get WiFi SSID/PASS"
        Me.Button3.UseVisualStyleBackColor = True
        '
        'Button4
        '
        Me.Button4.Enabled = False
        Me.Button4.Location = New System.Drawing.Point(12, 558)
        Me.Button4.Name = "Button4"
        Me.Button4.Size = New System.Drawing.Size(143, 23)
        Me.Button4.TabIndex = 45
        Me.Button4.TabStop = False
        Me.Button4.Text = "Save WiFi SSID/PASS"
        Me.Button4.UseVisualStyleBackColor = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(984, 596)
        Me.Controls.Add(Me.Button4)
        Me.Controls.Add(Me.Button3)
        Me.Controls.Add(Me.btn_reboot)
        Me.Controls.Add(Me.textbox_pass)
        Me.Controls.Add(Me.textbox_ssid)
        Me.Controls.Add(Me.Label11)
        Me.Controls.Add(Me.Label10)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.updown_wifichan)
        Me.Controls.Add(Me.Label8)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.updown_xclk)
        Me.Controls.Add(Me.Label6)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.updown_jpeg)
        Me.Controls.Add(Me.btn_aec2)
        Me.Controls.Add(Me.btn_cpu160)
        Me.Controls.Add(Me.btn_cpu80)
        Me.Controls.Add(Me.btn_cpu240)
        Me.Controls.Add(Me.btn_vflip)
        Me.Controls.Add(Me.btn_hflip)
        Me.Controls.Add(Me.btn_res6)
        Me.Controls.Add(Me.btn_res5)
        Me.Controls.Add(Me.btn_res4)
        Me.Controls.Add(Me.btn_res3)
        Me.Controls.Add(Me.btn_res2)
        Me.Controls.Add(Me.btn_res1)
        Me.Controls.Add(Me.btn_wifi_pwr_10)
        Me.Controls.Add(Me.btn_wifi_pwr_9)
        Me.Controls.Add(Me.btn_wifi_pwr_8)
        Me.Controls.Add(Me.btn_wifi_pwr_7)
        Me.Controls.Add(Me.btn_wifi_pwr_6)
        Me.Controls.Add(Me.btn_wifi_pwr_5)
        Me.Controls.Add(Me.btn_wifi_pwr_4)
        Me.Controls.Add(Me.btn_wifi_pwr_3)
        Me.Controls.Add(Me.btn_wifi_pwr_2)
        Me.Controls.Add(Me.btn_wifi_pwr_1)
        Me.Controls.Add(Me.Button2)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.PictureBox1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.updown_jpeg, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.updown_xclk, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.updown_wifichan, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents Button1 As Button
    Friend WithEvents Label1 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents timer_wifi As Timer
    Friend WithEvents PictureBox1 As PictureBox
    Friend WithEvents Label3 As Label
    Friend WithEvents timer_fps As Timer
    Friend WithEvents Label4 As Label
    Friend WithEvents Button2 As Button
    Friend WithEvents btn_wifi_pwr_1 As Button
    Friend WithEvents btn_wifi_pwr_2 As Button
    Friend WithEvents btn_wifi_pwr_3 As Button
    Friend WithEvents btn_wifi_pwr_4 As Button
    Friend WithEvents btn_wifi_pwr_5 As Button
    Friend WithEvents btn_wifi_pwr_6 As Button
    Friend WithEvents btn_wifi_pwr_7 As Button
    Friend WithEvents btn_wifi_pwr_8 As Button
    Friend WithEvents btn_wifi_pwr_9 As Button
    Friend WithEvents btn_wifi_pwr_10 As Button
    Friend WithEvents btn_res1 As Button
    Friend WithEvents btn_res2 As Button
    Friend WithEvents btn_res3 As Button
    Friend WithEvents btn_res4 As Button
    Friend WithEvents btn_res5 As Button
    Friend WithEvents btn_res6 As Button
    Friend WithEvents btn_hflip As Button
    Friend WithEvents btn_vflip As Button
    Friend WithEvents btn_cpu240 As Button
    Friend WithEvents btn_cpu80 As Button
    Friend WithEvents btn_cpu160 As Button
    Friend WithEvents btn_aec2 As Button
    Friend WithEvents updown_jpeg As NumericUpDown
    Friend WithEvents Label5 As Label
    Friend WithEvents Label6 As Label
    Friend WithEvents updown_xclk As NumericUpDown
    Friend WithEvents Label7 As Label
    Friend WithEvents Label8 As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents updown_wifichan As NumericUpDown
    Friend WithEvents Label10 As Label
    Friend WithEvents Label11 As Label
    Friend WithEvents textbox_ssid As TextBox
    Friend WithEvents textbox_pass As TextBox
    Friend WithEvents btn_reboot As Button
    Friend WithEvents timer_UDP_Connected As Timer
    Friend WithEvents Button3 As Button
    Friend WithEvents Button4 As Button
End Class
