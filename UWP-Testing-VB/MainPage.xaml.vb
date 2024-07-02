' The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

Imports MjpegProcessor

Imports Windows.Storage.Streams

''' <summary>
''' An empty page that can be used on its own or navigated to within a Frame.
''' </summary>
Public NotInheritable Class MainPage
    Inherits Page

    Private mjpeg As MjpegDecoder = New MjpegDecoder

    Public Sub New()
        InitializeComponent()

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

        ImageBox.Source = bitmapImage
    End Sub

    Private Sub btnStart_Click(sender As Object, e As RoutedEventArgs) Handles btnStart.Click
        mjpeg.ParseStream(New Uri("http://192.168.4.1:81/stream"))
        AddHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
    End Sub

    Private Sub btnStop_Click(sender As Object, e As RoutedEventArgs) Handles btnStop.Click
        mjpeg.StopStream()
        RemoveHandler mjpeg.FrameReady, AddressOf mjpeg_handleframe
        mjpeg = Nothing
    End Sub
End Class
