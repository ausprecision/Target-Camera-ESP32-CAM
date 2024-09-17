Imports System.Net
Imports System.Net.Sockets
Imports System.Text
Imports MjpegProcessor
Imports NativeWifi



Public Class Form1

    Public Shared UDPClient As UdpClient = New UdpClient()
    Public Shared UDPReceived As Boolean = False
    Public UDPThreadUDPReceive As System.Threading.Thread

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


    Public Async Sub UDPReceiveMessages()
        Debug.WriteLine("UDP Receieved Messages Running")
        Do While BoolExceptionLoop = False
            Try
                Dim bytRecieved As Byte() = UDPClient.Receive(ipRemoteEndPoint)
                UDPRecievedMessage = Encoding.ASCII.GetString(bytRecieved)
                Debug.WriteLine(UDPRecievedMessage)
                UDPRecievedMessage = UDPRecievedMessage.Replace(vbCr, "").Replace(vbLf, "")
                Invoke(Sub() Label2.Text = UDPRecievedMessage)
                UDPReceived = True
            Catch ex As Exception
                BoolExceptionLoop = True
                Debug.WriteLine("Error on UDP Listening")
                Debug.WriteLine(ex.ToString)
                Debug.WriteLine(ex.Message.ToString)
            End Try
            Await Task.Delay(1)
        Loop
    End Sub

    Public Shared Sub UDPSendMessages(CommandString As String)
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
        Label1.Text = WirelessPercentage(WirelessRSSI)
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        mjpeg.ParseStream(New Uri("http://192.168.4.1:81/stream"))
        AddHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
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
End Class
