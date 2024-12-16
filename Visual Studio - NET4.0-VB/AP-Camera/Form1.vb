Imports System.Net
Imports System.Net.Sockets
Imports System.Text
Imports System.Text.RegularExpressions
Imports MjpegProcessor
Imports NativeWifi

Public Class Form1
    Public bool_hflip = False
    Public bool_vflip = False
    Public bool_aec2 = False

    Public WiFiCHAN As Integer
    Public WiFiPOWR As String
    Public WiFiSSID As String
    Public WiFiPASS As String

    Public CameraRES As String

    Public Shared UDPClient As UdpClient = New UdpClient()
    Public Shared UDPReceived As Boolean = False
    Public UDPThreadUDPReceive As System.Threading.Thread
    Public fsRegular As FontStyle = FontStyle.Regular
    Public fsBold As FontStyle = FontStyle.Bold

    Public FontRegular = New Font(FontFamily.GenericSansSerif, 8.25, fsRegular)
    Public FontBold = New Font(FontFamily.GenericSansSerif, fsBold)

    Public BoolExceptionLoop As Boolean = False
    Public Shared UDPRecievedMessage As String = String.Empty
    Public ipRemoteEndPoint As IPEndPoint = New IPEndPoint(IPAddress.Any, 11000)

    Public wirelessNetworkName As String = ""

    Public Shared szStatus_Wifi As String = "[WiFi] Initializing..."

    Public fps As Integer = 0
    Private mjpeg As MjpegDecoder = New MjpegDecoder


    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Try
            UDPClient.Client.Bind(New IPEndPoint(IPAddress.Any, 2090))
            UDPClient.Connect(IPAddress.Parse("192.168.4.1"), 2090)
        Catch ex As Exception
            Debug.WriteLine("UDP Failed")
        End Try
        UDPThreadUDPReceive = New System.Threading.Thread(AddressOf UDPReceiveMessages)
        UDPThreadUDPReceive.Start()
    End Sub

    Public Async Sub ButtonsCheck(callingbutton As Button)
        Invoke(Sub()
                   btn_res1.Font = FontRegular
                   btn_res2.Font = FontRegular
                   btn_res3.Font = FontRegular
                   btn_res4.Font = FontRegular
                   btn_res5.Font = FontRegular
                   btn_res6.Font = FontRegular
                   callingbutton.Font = New Font(btn_res1.Font, FontStyle.Bold)
               End Sub)
        Await Task.Delay(1)
    End Sub

    Public Async Sub ButtonsDisable()
        Invoke(Sub()
                   btn_res1.Font = FontRegular
                   btn_res2.Font = FontRegular
                   btn_res3.Font = FontRegular
                   btn_res4.Font = FontRegular
                   btn_res5.Font = FontRegular
                   btn_res6.Font = FontRegular
                   btn_res1.Enabled = False
                   btn_res2.Enabled = False
                   btn_res3.Enabled = False
                   btn_res4.Enabled = False
                   btn_res5.Enabled = False
                   btn_res6.Enabled = False
               End Sub)
        Await Task.Delay(1)
    End Sub

    Public Async Sub ButtonsCPUDisable()
        Invoke(Sub()
                   btn_cpu240.Font = FontRegular
                   btn_cpu160.Font = FontRegular
                   btn_cpu80.Font = FontRegular
                   btn_cpu240.Enabled = False
                   btn_cpu160.Enabled = False
                   btn_cpu80.Enabled = False
               End Sub)
        Await Task.Delay(1)
    End Sub

    Public Async Sub ButtonsCPUCheck(callingbutton As Button)
        Invoke(Sub()
                   btn_cpu240.Font = FontRegular
                   btn_cpu160.Font = FontRegular
                   btn_cpu80.Font = FontRegular
                   callingbutton.Font = New Font(btn_res1.Font, FontStyle.Bold)
               End Sub)
        Await Task.Delay(1)
    End Sub
    Public Async Sub ButtonsWiFiCheck(callingbutton As Button)
        Invoke(Sub()
                   btn_wifi_pwr_1.Font = FontRegular
                   btn_wifi_pwr_2.Font = FontRegular
                   btn_wifi_pwr_3.Font = FontRegular
                   btn_wifi_pwr_4.Font = FontRegular
                   btn_wifi_pwr_5.Font = FontRegular
                   btn_wifi_pwr_6.Font = FontRegular
                   btn_wifi_pwr_7.Font = FontRegular
                   btn_wifi_pwr_8.Font = FontRegular
                   btn_wifi_pwr_9.Font = FontRegular
                   btn_wifi_pwr_10.Font = FontRegular
                   callingbutton.Font = New Font(btn_res1.Font, FontStyle.Bold)
               End Sub)
        Await Task.Delay(1)
    End Sub
    Public Async Sub ButtonsWiFiDisable()
        Invoke(Sub()
                   btn_wifi_pwr_1.Font = FontRegular
                   btn_wifi_pwr_2.Font = FontRegular
                   btn_wifi_pwr_3.Font = FontRegular
                   btn_wifi_pwr_4.Font = FontRegular
                   btn_wifi_pwr_5.Font = FontRegular
                   btn_wifi_pwr_6.Font = FontRegular
                   btn_wifi_pwr_7.Font = FontRegular
                   btn_wifi_pwr_8.Font = FontRegular
                   btn_wifi_pwr_9.Font = FontRegular
                   btn_wifi_pwr_10.Font = FontRegular
                   btn_wifi_pwr_1.Enabled = False
                   btn_wifi_pwr_2.Enabled = False
                   btn_wifi_pwr_3.Enabled = False
                   btn_wifi_pwr_4.Enabled = False
                   btn_wifi_pwr_5.Enabled = False
                   btn_wifi_pwr_6.Enabled = False
                   btn_wifi_pwr_7.Enabled = False
                   btn_wifi_pwr_8.Enabled = False
                   btn_wifi_pwr_9.Enabled = False
                   btn_wifi_pwr_10.Enabled = False
               End Sub)
        Await Task.Delay(1)
    End Sub


    Public Async Sub UDPReceiveMessages()

        Debug.WriteLine("UDP Receieved Messages Running")
        Do While BoolExceptionLoop = False
            Try
                Dim bytRecieved As Byte() = UDPClient.Receive(ipRemoteEndPoint)
                UDPRecievedMessage = Encoding.ASCII.GetString(bytRecieved)
                Debug.WriteLine(UDPRecievedMessage)

                If UDPRecievedMessage.Contains("[WIRELESS]") = True Then
                    Invoke(Sub()
                               Dim szUDPMessageArrayWireless() As String = Split(UDPRecievedMessage, "|")
                               textbox_ssid.Text = szUDPMessageArrayWireless(1).Trim
                               textbox_pass.Text = szUDPMessageArrayWireless(2).Trim
                               textbox_ssid.Enabled = True
                               textbox_pass.Enabled = True
                               Button3.Enabled = False
                               Button4.Enabled = True
                           End Sub)
                Else

                    Invoke(Sub() Label2.Text = UDPRecievedMessage)
                    UDPReceived = True

                    Dim szUDPMessageArray() As String = Split(UDPRecievedMessage, "|")

                    Select Case szUDPMessageArray(1).Substring(6).Trim
                        Case "VGA" : ButtonsCheck(btn_res1)
                        Case "SVGA" : ButtonsCheck(btn_res2)
                        Case "XGA" : ButtonsCheck(btn_res3)
                        Case "HD" : ButtonsCheck(btn_res4)
                        Case "SXGA" : ButtonsCheck(btn_res5)
                        Case "UXGA" : ButtonsCheck(btn_res6)
                    End Select

                    Select Case szUDPMessageArray(7).Substring(9).Trim
                        Case "5dBm" : ButtonsWiFiCheck(btn_wifi_pwr_1)
                        Case "7dBm" : ButtonsWiFiCheck(btn_wifi_pwr_2)
                        Case "8_5dBm" : ButtonsWiFiCheck(btn_wifi_pwr_3)
                        Case "11dBm" : ButtonsWiFiCheck(btn_wifi_pwr_4)
                        Case "13dBm" : ButtonsWiFiCheck(btn_wifi_pwr_5)
                        Case "15dBm" : ButtonsWiFiCheck(btn_wifi_pwr_6)
                        Case "17dBm" : ButtonsWiFiCheck(btn_wifi_pwr_7)
                        Case "18_5dBm" : ButtonsWiFiCheck(btn_wifi_pwr_8)
                        Case "19dBm" : ButtonsWiFiCheck(btn_wifi_pwr_9)
                        Case "19_5dBm" : ButtonsWiFiCheck(btn_wifi_pwr_10)
                    End Select

                    Select Case szUDPMessageArray(4).Substring(5).Trim
                        Case "240Mhz" : ButtonsCPUCheck(btn_cpu240)
                        Case "160Mhz" : ButtonsCPUCheck(btn_cpu160)
                        Case "80Mhz" : ButtonsCPUCheck(btn_cpu80)
                    End Select

                    Invoke(Sub()
                               btn_res1.Enabled = True
                               btn_res2.Enabled = True
                               btn_res3.Enabled = True
                               btn_res4.Enabled = True
                               btn_res5.Enabled = True
                               btn_res6.Enabled = True
                               btn_wifi_pwr_1.Enabled = True
                               btn_wifi_pwr_2.Enabled = True
                               btn_wifi_pwr_3.Enabled = True
                               btn_wifi_pwr_4.Enabled = True
                               btn_wifi_pwr_5.Enabled = True
                               btn_wifi_pwr_6.Enabled = True
                               btn_wifi_pwr_7.Enabled = True
                               btn_wifi_pwr_8.Enabled = True
                               btn_wifi_pwr_9.Enabled = True
                               btn_wifi_pwr_10.Enabled = True
                               btn_cpu240.Enabled = True
                               btn_cpu160.Enabled = True
                               btn_cpu80.Enabled = True
                               If Button4.Enabled = False Then Button3.Enabled = True

                           End Sub)

                    If Convert.ToInt32(szUDPMessageArray(9).Substring(6).Trim) = 1 Then
                        Invoke(Sub() btn_aec2.Text = "Auto Exposure [ON]")
                        Invoke(Sub() btn_aec2.Enabled = True)
                    Else
                        Invoke(Sub() btn_aec2.Text = "Auto Exposure [OFF]")
                        Invoke(Sub() btn_aec2.Enabled = True)
                    End If

                    If Convert.ToInt32(szUDPMessageArray(10).Substring(4).Trim) = 1 Then
                        Invoke(Sub() btn_hflip.Text = "Horizontal Flip [ON]")
                        Invoke(Sub() btn_hflip.Enabled = True)
                    Else
                        Invoke(Sub() btn_hflip.Text = "Horizontal Flip [OFF]")
                        Invoke(Sub() btn_hflip.Enabled = True)
                    End If

                    If Convert.ToInt32(szUDPMessageArray(11).Substring(4).Trim) = 1 Then
                        Invoke(Sub() btn_vflip.Text = "Vertical Flip [ON]")
                        Invoke(Sub() btn_vflip.Enabled = True)
                    Else
                        Invoke(Sub() btn_vflip.Text = "Vertical Flip [OFF]")
                        Invoke(Sub() btn_vflip.Enabled = True)
                    End If

                    Invoke(Sub()
                               updown_jpeg.Value = Convert.ToInt32(szUDPMessageArray(12).Substring(6).Trim)
                               updown_jpeg.Enabled = True
                           End Sub)

                    Invoke(Sub()
                               updown_xclk.Value = Convert.ToInt32(szUDPMessageArray(8).Substring(6).Trim)
                               updown_xclk.Enabled = True
                           End Sub)

                    Invoke(Sub()
                               updown_wifichan.Value = Convert.ToInt32(szUDPMessageArray(6).Substring(9).Trim)
                               updown_wifichan.Enabled = True
                           End Sub)
                End If




            Catch ex As Exception
                BoolExceptionLoop = True
                Debug.WriteLine("Error on UDP Listening")
                Debug.WriteLine(ex.ToString)
                Debug.WriteLine(ex.Message.ToString)
            End Try
            Await Task.Delay(1)
        Loop
    End Sub

    Public Shared Sub UDPSendMessage(CommandString As String)
        Try
            Dim data As Byte() = Encoding.ASCII.GetBytes(CommandString)
            UDPClient.Send(data, data.Length)
        Catch ex As Exception
            Debug.WriteLine("UDP: Error Sending Delay and Timing Data to Base Station")
        End Try
    End Sub



    Public Shared Function WirelessRSSI() As Integer
        WirelessRSSI = 999
        Dim client As New WlanClient
        Try
            For Each wlaniface As WlanClient.WlanInterface In client.Interfaces
                If wlaniface.InterfaceState.ToString = "Connected" Then
                    WirelessRSSI = wlaniface.RSSI
                End If
            Next
        Catch ex As Exception
            WirelessRSSI = 999
        End Try
    End Function

    Public Shared Function WirelessPercentage(RSSI As Integer) As String
        Dim RSSI_Adjust As Integer = RSSI
        If RSSI > 0 Then
            RSSI_Adjust = (100 / 256) * RSSI
        Else
            RSSI_Adjust = 100 + RSSI
        End If

        If RSSI = 999 Then
            WirelessPercentage = "LAN / Local Connection"
        Else
            WirelessPercentage = ("[WiFi] " & RSSI_Adjust.ToString & "% RSSI: " & RSSI.ToString & "(dbm)")
        End If
    End Function

    Private Sub timer_wifi_Tick(sender As Object, e As EventArgs) Handles timer_wifi.Tick
        Try
            Label1.Text = WirelessPercentage(WirelessRSSI)
        Catch ex As Exception
            'MsgBox("WiFi State Interupted!")
        End Try

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If timer_fps.Enabled = True Then
            timer_fps.Stop()
            fps = 0
            timer_fps.Enabled = False
        End If
        mjpeg.ParseStream(New Uri("http://192.168.4.1:81/stream"))
        AddHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
        timer_fps.Enabled = True
        timer_fps.Start()

    End Sub


    Private Async Sub mjpeg_handleframe(ByVal sender As Object, ByVal e As FrameReadyEventArgs)
        Dim bitmapImage As Bitmap
        'Using stream = New InMemoryRandomAccessStream()
        bitmapImage = e.Bitmap.Clone()
        PictureBox1.Image = bitmapImage
        Label3.Text = "[Resolution] " & bitmapImage.Width.ToString & "x" & bitmapImage.Height.ToString
        fps = fps + 1
        Await Task.Delay(1)
    End Sub

    Private Sub timer_fps_Tick(sender As Object, e As EventArgs) Handles timer_fps.Tick
        Label4.Text = "[FPS] " & Convert.ToDecimal(fps / 10)
        fps = 0
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Dim filename As String = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) & "\AUS Precision\" & Now.ToString("yyyy-MM-dd-HHmmss") & ".bmp"
        PictureBox1.Image.Save(filename)
    End Sub

    Private Sub btn_wifi_pwr_1_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_1.Click
        UDPSendMessage("wireless set power 1")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_2_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_2.Click
        UDPSendMessage("wireless set power 2")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_3_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_3.Click
        UDPSendMessage("wireless set power 3")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_4_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_4.Click
        UDPSendMessage("wireless set power 4")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_5_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_5.Click
        UDPSendMessage("wireless set power 5")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_6_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_6.Click
        UDPSendMessage("wireless set power 6")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_7_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_7.Click
        UDPSendMessage("wireless set power 7")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_8_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_8.Click
        UDPSendMessage("wireless set power 8")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_9_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_9.Click
        UDPSendMessage("wireless set power 9")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_wifi_pwr_10_Click(sender As Object, e As EventArgs) Handles btn_wifi_pwr_10.Click
        UDPSendMessage("wireless set power 10")
        ButtonsWiFiDisable()
    End Sub

    Private Sub btn_res1_Click(sender As Object, e As EventArgs) Handles btn_res1.Click
        UDPSendMessage("camera set res VGA")
        ButtonsDisable()
    End Sub

    Private Sub btn_res2_Click(sender As Object, e As EventArgs) Handles btn_res2.Click
        UDPSendMessage("camera set res SVGA")
        ButtonsDisable()
    End Sub

    Private Sub btn_res3_Click(sender As Object, e As EventArgs) Handles btn_res3.Click
        UDPSendMessage("camera set res XGA")
        ButtonsDisable()
    End Sub

    Private Sub btn_res4_Click(sender As Object, e As EventArgs) Handles btn_res4.Click
        UDPSendMessage("camera set res HD")
        ButtonsDisable()
    End Sub

    Private Sub btn_res5_Click(sender As Object, e As EventArgs) Handles btn_res5.Click
        UDPSendMessage("camera set res SXGA")
        ButtonsDisable()
    End Sub

    Private Sub btn_res6_Click(sender As Object, e As EventArgs) Handles btn_res6.Click
        UDPSendMessage("camera set res UXGA")
        ButtonsDisable()
    End Sub

    Private Sub btn_cpu240_Click(sender As Object, e As EventArgs) Handles btn_cpu240.Click
        UDPSendMessage("cpu set 240")
        ButtonsCPUDisable()
    End Sub

    Private Sub btn_cpu160_Click(sender As Object, e As EventArgs) Handles btn_cpu160.Click
        UDPSendMessage("cpu set 160")
        ButtonsCPUDisable()
    End Sub

    Private Sub btn_cpu80_Click(sender As Object, e As EventArgs) Handles btn_cpu80.Click
        UDPSendMessage("cpu set 80")
        ButtonsCPUDisable()
    End Sub

    Private Sub timer_UDP_Connected_Tick(sender As Object, e As EventArgs) Handles timer_UDP_Connected.Tick
        If UDPClient.Client.IsBound = False Then
            UDPReceived = False
            MsgBox("Disconnected from Camera UDP Server / No Connection to Camera!")
        ElseIf UDPClient.Client.Connected = True And UDPReceived = True Then
        End If
    End Sub

    Private Sub btn_hflip_Click(sender As Object, e As EventArgs) Handles btn_hflip.Click
        If btn_hflip.Text = "Horizontal Flip [ON]" Then
            UDPSendMessage("camera set hflip off")
            btn_hflip.Text = "Horizontal Flip"
            btn_hflip.Enabled = False
        Else
            UDPSendMessage("camera set hflip on")
            btn_hflip.Enabled = False
            btn_hflip.Text = "Horizontal Flip"
        End If
    End Sub

    Private Sub btn_vflip_Click(sender As Object, e As EventArgs) Handles btn_vflip.Click
        If btn_vflip.Text = "Vertical Flip [ON]" Then
            UDPSendMessage("camera set vflip off")
            btn_vflip.Text = "Vertical Flip"
            btn_vflip.Enabled = False
        Else
            UDPSendMessage("camera set vflip on")
            btn_vflip.Enabled = False
            btn_vflip.Text = "Vertical Flip"
        End If
    End Sub

    Private Sub btn_aec2_Click(sender As Object, e As EventArgs) Handles btn_aec2.Click
        If btn_aec2.Text = "Auto Exposure [ON]" Then
            UDPSendMessage("camera set aec2 off")
            btn_aec2.Text = "Auto Exposure"
            btn_aec2.Enabled = False
        Else
            UDPSendMessage("camera set aec2 on")
            btn_aec2.Enabled = False
            btn_aec2.Text = "Auto Exposure"
        End If
    End Sub

    Private Sub btn_reboot_Click(sender As Object, e As EventArgs) Handles btn_reboot.Click
        UDPSendMessage("reboot")
        MsgBox("Rebooting in 5 Seconds!")

        btn_wifi_pwr_1.Enabled = False
        btn_wifi_pwr_2.Enabled = False
        btn_wifi_pwr_3.Enabled = False
        btn_wifi_pwr_4.Enabled = False
        btn_wifi_pwr_5.Enabled = False
        btn_wifi_pwr_6.Enabled = False
        btn_wifi_pwr_7.Enabled = False
        btn_wifi_pwr_8.Enabled = False
        btn_wifi_pwr_9.Enabled = False
        btn_wifi_pwr_10.Enabled = False

        btn_res1.Enabled = False
        btn_res2.Enabled = False
        btn_res3.Enabled = False
        btn_res4.Enabled = False
        btn_res5.Enabled = False
        btn_res6.Enabled = False

        btn_hflip.Enabled = False
        btn_hflip.Text = "Horizontal Flip"
        btn_vflip.Enabled = False
        btn_vflip.Text = "Vertical Flip"
        btn_aec2.Enabled = False
        btn_aec2.Text = "Auto Exposure"
    End Sub

    Private Sub updown_jpeg_ValueChanged(sender As Object, e As EventArgs) Handles updown_jpeg.ValueChanged
        If updown_jpeg.Enabled = True Then UDPSendMessage("camera set quality " & updown_jpeg.Value.ToString)
        updown_jpeg.Enabled = False
    End Sub

    Private Sub updown_xclk_ValueChanged(sender As Object, e As EventArgs) Handles updown_xclk.ValueChanged
        If updown_xclk.Enabled = True Then UDPSendMessage("camera set xclk " & updown_xclk.Value.ToString)
        updown_xclk.Enabled = False
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        UDPSendMessage("wireless get config")
        Button3.Enabled = False
    End Sub

    Private Sub updown_wifichan_ValueChanged(sender As Object, e As EventArgs) Handles updown_wifichan.ValueChanged
        If updown_wifichan.Enabled = True Then UDPSendMessage("wireless set channel " & updown_wifichan.Value.ToString)
        If updown_wifichan.Enabled = True Then MsgBox("Wireless CHAN Has Changed, Reboot Required!")
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        UDPSendMessage("wireless set password " & textbox_pass.Text)
        UDPSendMessage("wireless set name " & textbox_ssid.Text)
        textbox_pass.Enabled = False
        textbox_ssid.Enabled = False
        Button4.Enabled = False
    End Sub
End Class
