' The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

Imports MjpegProcessor

Imports Windows.Storage.Streams
Imports Windows.Devices.WiFi
Imports Windows.Devices
Imports System.Net.Sockets
Imports System.Net
Imports System.Text
Imports ManagedNativeWifi

''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage
    Inherits Page

    Public fpstimer As DispatcherTimer
    Public status_timer As DispatcherTimer

    Public fps As Integer = 0
    Private mjpeg As MjpegDecoder = New MjpegDecoder


    Public Sub New()
        InitializeComponent()
        fpstimer = New DispatcherTimer
        fpstimer.Interval = TimeSpan.FromMilliseconds(10000)
        AddHandler fpstimer.Tick, AddressOf fpstimer_Tick

        status_timer = New DispatcherTimer
        status_timer.Interval = TimeSpan.FromMilliseconds(1)
        AddHandler status_timer.Tick, AddressOf status_timer_Tick
        status_timer.Start()

        textbox_command.IsSpellCheckEnabled = False

    End Sub

    Private Async Sub mjpeg_handleframe(ByVal sender As Object, ByVal e As FrameReadyEventArgs)
        Dim bitmapImage = New BitmapImage()
        Using stream = New InMemoryRandomAccessStream()

            Using writer = New DataWriter(stream)
                writer.WriteBytes(e.FrameBuffer.Clone())
                Await writer.StoreAsync()
                Await writer.FlushAsync()
                writer.DetachStream()
            End Using

            stream.Seek(0)
            Await bitmapImage.SetSourceAsync(stream)
        End Using
        Label1.Text = "MJPEG Stream Resolution: " & bitmapImage.PixelWidth.ToString & "x" & bitmapImage.PixelHeight.ToString
        fps = fps + 1
        ImageBox.Source = bitmapImage
    End Sub

    Private Sub btnStart_Click(sender As Object, e As RoutedEventArgs) Handles btnStart.Click
        mjpeg.ParseStream(New Uri("http://192.168.4.1:81/stream"))
        AddHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
        fpstimer.Start()
    End Sub

    Private Sub btnStop_Click(sender As Object, e As RoutedEventArgs) Handles btnStop.Click
        mjpeg.StopStream()
        fpstimer.Stop()
        RemoveHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
        mjpeg = Nothing
    End Sub

    Private Sub btnStart_ap_Click(sender As Object, e As RoutedEventArgs) Handles btnStart_ap.Click
        mjpeg.ParseStream(New Uri("http://stream.ausprecision.com.au:8080"))
        AddHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
        fpstimer.Start()
    End Sub

    Private Sub fpstimer_Tick(ByVal sender As Object, ByVal e As Object)
        Label2.Text = "Frames Per Second (FPS): " & Convert.ToDecimal(fps / 5)
        fps = 0
    End Sub

    Private Sub button_setres_Click(sender As Object, e As RoutedEventArgs) Handles button_setres.Click
        App.UDPSendMessages("camera set res SVGA")
    End Sub

    Private Sub status_timer_Tick(ByVal sender As Object, ByVal e As Object)
        lbl_status_wifi.Text = App.szStatus_Wifi
        If App.UDPReceived = True Then lbl_status_camera.Text = "[Camera] " & App.UDPRecievedMessage
    End Sub

    Private Sub button_send_command_Click(sender As Object, e As RoutedEventArgs) Handles button_send_command.Click
        App.UDPSendMessages(textbox_command.Text)
    End Sub
End Class
