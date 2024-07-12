' The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

Imports MjpegProcessor

Imports Windows.Storage.Streams
Imports Windows.Devices.WiFi
Imports Windows.Devices

''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage
    Inherits Page

    Public fpstimer As DispatcherTimer
    Public fps As Integer = 0

    Public wiFiAdapters As WiFiAdapter







    Private mjpeg As MjpegDecoder = New MjpegDecoder

    Public Sub New()



        InitializeComponent()
        fpstimer = New DispatcherTimer
        fpstimer.Interval = TimeSpan.FromMilliseconds(5000)
        AddHandler fpstimer.Tick, AddressOf fpstimer_Tick

        GetWiFiAdapter()


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
        GetWiFiAdapter()

    End Sub


    Public Async Sub GetWiFiAdapter()
        Dim firstAdapter As WiFiAdapter
        Dim result = Await Windows.Devices.Enumeration.DeviceInformation.FindAllAsync(WiFiAdapter.GetDeviceSelector())
        If result.Count >= 1 Then
            firstAdapter = Await WiFiAdapter.FromIdAsync(result(0).Id)
            Await firstAdapter.ScanAsync()
            Dim wifis = firstAdapter.NetworkReport.AvailableNetworks



            For Each item In wifis
                If item.Ssid = "Enclypse" Then
                    Debug.Write("WiFi Percentage = " & (Math.Min(Math.Max(2 * (item.NetworkRssiInDecibelMilliwatts + 100), 0), 100)).ToString)
                    Debug.WriteLine(String.Format(" | {0}dBm", item.NetworkRssiInDecibelMilliwatts))
                End If
            Next

        End If
    End Sub

End Class
