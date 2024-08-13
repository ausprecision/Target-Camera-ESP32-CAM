Imports System.Net
Imports System.Net.NetworkInformation

Imports System.Net.Sockets
Imports System.Text
Imports Windows.Devices.WiFi

''' <summary>
''' Provides application-specific behavior to supplement the default Application class.
''' </summary>
NotInheritable Class App
    Inherits Application

    Public Shared UDPClient As UdpClient = New UdpClient()
    Public Shared UDPReceived As Boolean = False
    Public UDPThreadUDPReceive As System.Threading.Thread

    Public BoolExceptionLoop As Boolean = False
    Public Shared UDPRecievedMessage As String = String.Empty
    Public ipRemoteEndPoint As IPEndPoint = New IPEndPoint(IPAddress.Any, 11000)

    Public ConnectionWiFiTimer As DispatcherTimer
    Public Shared ConnectionWiFiConnected As Boolean = False
    Public Shared ConnectionBaseStationConnected As Boolean = False
    Public wirelessAdapter As WiFiAdapter
    Public wirelessNetworkName As String = ""

    Public Shared szStatus_Wifi As String = "[WiFi] Initializing..."

    ''' <summary>
    ''' Invoked when the application is launched normally by the end user.  Other entry points
    ''' will be used when the application is launched to open a specific file, to display
    ''' search results, and so forth.
    ''' </summary>
    ''' <param name="e">Details about the launch request and process.</param>
    Protected Overrides Async Sub OnLaunched(e As Windows.ApplicationModel.Activation.LaunchActivatedEventArgs)
        Dim rootFrame As Frame = TryCast(Window.Current.Content, Frame)
        ' Do not repeat app initialization when the Window already has content,
        ' just ensure that the window is active
        If rootFrame Is Nothing Then
            ' Create a Frame to act as the navigation context and navigate to the first page
            rootFrame = New Frame()

            AddHandler rootFrame.NavigationFailed, AddressOf OnNavigationFailed

            If e.PreviousExecutionState = ApplicationExecutionState.Terminated Then
                ' TODO: Load state from previously suspended application
            End If
            ' Place the frame in the current Window
            Window.Current.Content = rootFrame
        End If
        If e.PrelaunchActivated = False Then
            If rootFrame.Content Is Nothing Then
                ' When the navigation stack isn't restored navigate to the first page,
                ' configuring the new page by passing required information as a navigation
                ' parameter
                rootFrame.Navigate(GetType(MainPage), e.Arguments)
            End If

            ' Ensure the current window is active
            Window.Current.Activate()
        End If

        UDPClient.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, True)
        UDPClient.Client.Bind(New IPEndPoint(IPAddress.Any, 2090))
        UDPClient.Connect(IPAddress.Parse("192.168.4.1"), 2090)

        UDPThreadUDPReceive = New System.Threading.Thread(AddressOf UDPReceiveMessages)
        UDPThreadUDPReceive.Start()


        ConnectionWiFiTimer = New DispatcherTimer
        ConnectionWiFiTimer.Interval = TimeSpan.FromMilliseconds(10000)
        AddHandler ConnectionWiFiTimer.Tick, AddressOf ConnectionWiFiTimer_Tick
        ConnectionWiFiTimer.Start()

    End Sub

    ''' <summary>
    ''' Invoked when Navigation to a certain page fails
    ''' </summary>
    ''' <param name="sender">The Frame which failed navigation</param>
    ''' <param name="e">Details about the navigation failure</param>
    Private Sub OnNavigationFailed(sender As Object, e As NavigationFailedEventArgs)
        Throw New Exception("Failed to load Page " + e.SourcePageType.FullName)
    End Sub

    ''' <summary>
    ''' Invoked when application execution is being suspended.  Application state is saved
    ''' without knowing whether the application will be terminated or resumed with the contents
    ''' of memory still intact.
    ''' </summary>
    ''' <param name="sender">The source of the suspend request.</param>
    ''' <param name="e">Details about the suspend request.</param>
    Private Sub OnSuspending(sender As Object, e As SuspendingEventArgs) Handles Me.Suspending
        Dim deferral As SuspendingDeferral = e.SuspendingOperation.GetDeferral()
        ' TODO: Save application state and stop any background activity
        deferral.Complete()
    End Sub

    Public Async Sub UDPReceiveMessages()
        Do While BoolExceptionLoop = False
            Try
                Dim bytRecieved As Byte() = UDPClient.Receive(ipRemoteEndPoint)
                UDPRecievedMessage = Encoding.ASCII.GetString(bytRecieved)
                Debug.WriteLine(UDPRecievedMessage)
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



    Private Async Sub ConnectionWiFiTimer_Tick(ByVal sender As Object, ByVal e As Object)
        Try
            Dim result = Await Windows.Devices.Enumeration.DeviceInformation.FindAllAsync(WiFiAdapter.GetDeviceSelector())
            If result.Count >= 1 Then
                For x = 0 To result.Count - 1
                    wirelessAdapter = Await WiFiAdapter.FromIdAsync(result(x).Id)
                    Dim connectedprofile = Await wirelessAdapter.NetworkAdapter.GetConnectedProfileAsync()
                    If connectedprofile IsNot Nothing Then
                        wirelessNetworkName = connectedprofile.ProfileName
                        ConnectionWiFiConnected = True
                        Debug.WriteLine("[WiFi] SSID: " & wirelessNetworkName & " | " & (Math.Min(Math.Max(2 * (wirelessAdapter.NetworkReport.AvailableNetworks(0).NetworkRssiInDecibelMilliwatts + 100), 0), 100)).ToString & "%" & " (RSSI: " & wirelessAdapter.NetworkReport.AvailableNetworks(0).NetworkRssiInDecibelMilliwatts & "dbm)")
                        szStatus_Wifi = "[WiFi] SSID: " & wirelessNetworkName & " | " & (Math.Min(Math.Max(2 * (wirelessAdapter.NetworkReport.AvailableNetworks(0).NetworkRssiInDecibelMilliwatts + 100), 0), 100)).ToString & "%" & " (RSSI: " & wirelessAdapter.NetworkReport.AvailableNetworks(0).NetworkRssiInDecibelMilliwatts & "dbm)"
                        Exit For
                    Else
                        ConnectionWiFiConnected = False
                    End If
                Next
            Else
                Debug.WriteLine("[WiFi] - Not Connected / Local LAN Connection")
                szStatus_Wifi = "[WiFi] - Not Connected / Local LAN Connection"
            End If
        Catch ex As Exception
            Debug.WriteLine("[WiFi] Error - Failed to Retrieve WiFi Network Information")
            szStatus_Wifi = "[WiFi] Error - Failed to Retrieve WiFi Network Information"
            Debug.WriteLine(ex.ToString)
            Debug.WriteLine(ex.Message.ToString)
        End Try
    End Sub

End Class
