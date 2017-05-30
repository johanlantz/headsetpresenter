Imports Franson.BlueTools
Imports Franson.Protocols.Obex
Imports Franson.Protocols.Obex.ObjectPush
Imports System.IO

Public Class MainForm
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then

            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents copyPanel As System.Windows.Forms.StatusBarPanel
    Friend WithEvents openFileDialog As System.Windows.Forms.OpenFileDialog
    Friend WithEvents statusBarPanel As System.Windows.Forms.StatusBarPanel
    Friend WithEvents informLabel As System.Windows.Forms.Label
    Friend WithEvents statusBar As System.Windows.Forms.StatusBar
    Friend WithEvents progressBar As System.Windows.Forms.ProgressBar
    Friend WithEvents sendBtn As System.Windows.Forms.Button
    Friend WithEvents discoverBtn As System.Windows.Forms.Button
    Friend WithEvents label1 As System.Windows.Forms.Label
    Friend WithEvents deviceList As System.Windows.Forms.ListBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.copyPanel = New System.Windows.Forms.StatusBarPanel
        Me.openFileDialog = New System.Windows.Forms.OpenFileDialog
        Me.statusBarPanel = New System.Windows.Forms.StatusBarPanel
        Me.informLabel = New System.Windows.Forms.Label
        Me.statusBar = New System.Windows.Forms.StatusBar
        Me.progressBar = New System.Windows.Forms.ProgressBar
        Me.sendBtn = New System.Windows.Forms.Button
        Me.discoverBtn = New System.Windows.Forms.Button
        Me.label1 = New System.Windows.Forms.Label
        Me.deviceList = New System.Windows.Forms.ListBox
        CType(Me.copyPanel, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.statusBarPanel, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'copyPanel
        '
        Me.copyPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents
        Me.copyPanel.Width = 10
        '
        'openFileDialog
        '
        Me.openFileDialog.Multiselect = True
        '
        'statusBarPanel
        '
        Me.statusBarPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring
        Me.statusBarPanel.Width = 180
        '
        'informLabel
        '
        Me.informLabel.Location = New System.Drawing.Point(16, 244)
        Me.informLabel.Name = "informLabel"
        Me.informLabel.Size = New System.Drawing.Size(160, 40)
        Me.informLabel.TabIndex = 13
        Me.informLabel.Text = "Click Discover to find nearby Bluetooth devices."
        '
        'statusBar
        '
        Me.statusBar.Location = New System.Drawing.Point(0, 293)
        Me.statusBar.Name = "statusBar"
        Me.statusBar.Panels.AddRange(New System.Windows.Forms.StatusBarPanel() {Me.statusBarPanel, Me.copyPanel})
        Me.statusBar.ShowPanels = True
        Me.statusBar.Size = New System.Drawing.Size(190, 22)
        Me.statusBar.SizingGrip = False
        Me.statusBar.TabIndex = 12
        '
        'progressBar
        '
        Me.progressBar.Location = New System.Drawing.Point(16, 196)
        Me.progressBar.Name = "progressBar"
        Me.progressBar.Size = New System.Drawing.Size(160, 8)
        Me.progressBar.TabIndex = 11
        '
        'sendBtn
        '
        Me.sendBtn.Enabled = False
        Me.sendBtn.Location = New System.Drawing.Point(104, 212)
        Me.sendBtn.Name = "sendBtn"
        Me.sendBtn.TabIndex = 10
        Me.sendBtn.Text = "Push"
        '
        'discoverBtn
        '
        Me.discoverBtn.Location = New System.Drawing.Point(16, 212)
        Me.discoverBtn.Name = "discoverBtn"
        Me.discoverBtn.TabIndex = 9
        Me.discoverBtn.Text = "Discover"
        '
        'label1
        '
        Me.label1.Location = New System.Drawing.Point(16, 4)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(100, 16)
        Me.label1.TabIndex = 8
        Me.label1.Text = "Devices"
        '
        'deviceList
        '
        Me.deviceList.Location = New System.Drawing.Point(16, 28)
        Me.deviceList.Name = "deviceList"
        Me.deviceList.Size = New System.Drawing.Size(160, 160)
        Me.deviceList.TabIndex = 7
        '
        'MainForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(190, 315)
        Me.Controls.Add(Me.statusBar)
        Me.Controls.Add(Me.progressBar)
        Me.Controls.Add(Me.sendBtn)
        Me.Controls.Add(Me.discoverBtn)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.deviceList)
        Me.Controls.Add(Me.informLabel)
        Me.Name = "MainForm"
        Me.Text = "ObjectPush Sample"
        CType(Me.copyPanel, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.statusBarPanel, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

#End Region

    Dim m_manager As Manager
    Dim WithEvents m_network As Network
    Dim m_serviceCurrent As RemoteService
    Dim WithEvents m_deviceCurrent As RemoteDevice
    Dim WithEvents m_objectPush As ObexObjectPush
    Dim m_streamCurrent As ServiceStream
    Dim WithEvents m_deviceSelected As RemoteDevice

    Dim m_intTransfers As Integer

    Dim m_boolAborted As Boolean
    Dim m_boolDenied As Boolean
    Dim m_boolUnrecoverableError As Boolean

    Private Sub discoverBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles discoverBtn.Click

        ' Start looking for devices on the network
        ' Use Network.DiscoverDevices() if you don't want to use events.
        m_network.DiscoverDevicesAsync()

    End Sub

    Private Sub sendBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles sendBtn.Click

        If sendBtn.Text.Equals("Push") Then

            Try

                ' if we have a service to send to
                If Not m_serviceCurrent Is Nothing Then

                    ' make sure ObexObjectPush is initialized and select file to push to device
                    If Not m_objectPush Is Nothing And openFileDialog.ShowDialog() = DialogResult.OK Then

                        For Each filename As String In openFileDialog.FileNames

                            m_streamCurrent = m_serviceCurrent.Stream

                            Dim fileStream As FileStream

                            fileStream = New FileStream(filename, FileMode.Open, FileAccess.Read)
                            m_objectPush.PushFileAsync(fileStream, Path.GetFileName(filename), m_streamCurrent)

                            ' Increment number of transfers
                            m_intTransfers = m_intTransfers + 1

                        Next
                    End If
                End If
            Catch

                ' disable Push button if there was an error
                sendBtn.Enabled = False

                informLabel.Text = "Failed to get a socket. Click on the device you wish to push a file to."

            End Try
        Else

            ' if button text isn't Send it will be Cancel - 
            ' if user click it, we tell ObexObjectPush that we want to abort
            m_objectPush.Abort()

            ' only need to press cancel once - won't help you to press it more
            sendBtn.Enabled = False

        End If
    End Sub

    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        ' catch all BlueTools exceptions - like e.g. license expired
        Try

            ' setup license for bluetools - use license key given
            Dim License As Franson.BlueTools.License
            License = New Franson.BlueTools.License

            License.LicenseKey = "WoK6IL944A9CIONQRXaYUjpRJRiuHYFYWrT7"

            ' get bluetools manager
            m_manager = Manager.GetManager()

            ' get first network dongle - bluetools 1.0 only supports one!
            m_network = m_manager.Networks(0)

            ' marshal events into this class thread.
            m_manager.Parent = Me

            ' update statusbar panel with name of currently used stack
            Select Case Manager.StackID

                Case StackID.STACK_MICROSOFT

                    statusBarPanel.Text = "Microsoft Stack"

                Case StackID.STACK_WIDCOMM

                    statusBarPanel.Text = "Widcomm Stack"

                Case Else

                    statusBarPanel.Text = "Unknown stack"

            End Select



        Catch exc As Exception

            ' display BlueTools errors here
            MessageBox.Show(exc.Message)

        End Try
    End Sub

    Private Sub MainForm_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing


        'if currentService is available, close the stream
        If Not m_serviceCurrent Is Nothing Then

            If Not m_streamCurrent Is Nothing Then

                m_streamCurrent.Close()

                ' set these objects to null to mark them for GC
                m_streamCurrent = Nothing
                m_serviceCurrent = Nothing
            End If
        End If

        ' must dispose manager else application can't exit!
        If Not m_manager Is Nothing Then

            m_manager.Dispose()

        End If

    End Sub

    Private Sub m_network_DeviceDiscovered(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_network.DeviceDiscovered

        ' add every device found
        Dim device As RemoteDevice
        Dim discovery As DiscoveryEventArgs
        discovery = DirectCast(eventArgs, DiscoveryEventArgs)
        device = DirectCast(discovery.Discovery, RemoteDevice)
        deviceList.Items.Add(device)

    End Sub

    Private Sub m_network_DeviceDiscoveryStarted(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_network.DeviceDiscoveryStarted

        ' disable the Discover button when we begin device discovery
        discoverBtn.Enabled = False

        ' disable the send button since you can't send while device discovering
        sendBtn.Enabled = False

        ' disable the device list while searching for devices
        deviceList.Enabled = False

        ' inform the user what we're doing
        informLabel.Text = "Scanning for devices..."
    End Sub

    Private Sub m_network_DeviceDiscoveryCompleted(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_network.DeviceDiscoveryCompleted

        Dim discovery As DiscoveryEventArgs
        discovery = DirectCast(eventArgs, DiscoveryEventArgs)

        Dim devices As Device() = DirectCast(discovery.Discovery, Device())

        ' enable Discover button again since device discovery is complete
        discoverBtn.Enabled = True

        ' enable the device list again
        deviceList.Enabled = True

        ' inform the user what to do
        informLabel.Text = "Click on the device you wish to push a file to."
    End Sub

    Private Sub deviceList_SelectedIndexChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles deviceList.SelectedIndexChanged

        ' get selected item (a remote device)
        m_deviceSelected = DirectCast(deviceList.SelectedItem, RemoteDevice)

        ' detect Object Push service (OPP) on this device

        deviceList.Enabled = False
        m_deviceSelected.DiscoverServicesAsync(ServiceType.OBEXObjectPush)

    End Sub

    Private Sub selectedDevice_ServiceDiscoveryStarted(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_deviceSelected.ServiceDiscoveryStarted

        ' when beginning to search for services - disable the Discover button
        ' we shouldn't scan for devices while attempting to scan for services.
        discoverBtn.Enabled = False
        sendBtn.Enabled = False
    End Sub

    Private Sub selectedDevice_ServiceDiscoveryCompleted(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_deviceSelected.ServiceDiscoveryCompleted

        ' when service discovery is complete - let us re-enable the discover button. 
        ' It is okay to try to update the device list
        discoverBtn.Enabled = True

        ' re-enable the device list
        deviceList.Enabled = True

        ' set error event handler for the device
        Dim device As RemoteDevice = DirectCast(sender, RemoteDevice)

        ' get all services found
        Dim discovery As DiscoveryEventArgs
        discovery = DirectCast(eventArgs, DiscoveryEventArgs)

        Dim services As Service() = DirectCast(discovery.Discovery, Service())

        ' if we have a service since before, close the stream
        If Not m_streamCurrent Is Nothing Then

            m_streamCurrent.Close()
            m_streamCurrent = Nothing
        End If

        ' and remove the service
        If Not m_serviceCurrent Is Nothing Then

            m_serviceCurrent = Nothing
        End If

        ' if we found a new service...
        If services.Length > 0 Then

            ' ...get OPP service object
            m_serviceCurrent = DirectCast(services(0), RemoteService)

            Try

                ' create an ObexObjectPush object connected to the ServiceStream
                m_objectPush = New ObexObjectPush(-1) ' wait forever

                ' marshal event to this class thread
                m_objectPush.Parent = Me

                ' enable the push button
                sendBtn.Enabled = True

                ' inform the user of the next step
                informLabel.Text = "Click on Push to select file(s) to push to device."

            Catch exc As Exception

                sendBtn.Enabled = False

                MessageBox.Show(exc.Message)

            End Try
        End If
    End Sub

    Private Sub m_objectPush_PutFileProgress(ByVal sender As Object, ByVal eventArgs As ObexEventArgs) Handles m_objectPush.PutFileProgress

        Dim copyArgs As ObexCopyEventArgs = DirectCast(eventArgs, ObexCopyEventArgs)

        ' if size isn't unknown
        If Not copyArgs.Size = -1 Then
            ' if max value on progressbar isn't set, set it
            If Not copyArgs.Size = progressBar.Maximum Then
                progressBar.Maximum = copyArgs.Size
            End If
            ' set position of file copy
            progressBar.Value = copyArgs.Position

            ' update copy panel to show progress in kb
            copyPanel.Text = Convert.ToString(copyArgs.Position \ 1024) + " kb / " + Convert.ToString(copyArgs.Size \ 1024) + " kb"

        Else

            ' update copy panel to show progress in kb
            copyPanel.Text = Convert.ToString(copyArgs.Position \ 1024) + " kb / ? kb"

        End If

    End Sub

    Private Sub m_objectPush_PutFileEnd(ByVal sender As Object, ByVal eventArgs As ObexEventArgs) Handles m_objectPush.PutFileEnd
        ' when finished copying...

        ' ...close file stream
        Dim copyArgs As ObexCopyEventArgs = DirectCast(eventArgs, ObexCopyEventArgs)

        ' Close Stream opened by ObexObjectPush
        copyArgs.Stream.Close()

        ' ...enable discover button
        discoverBtn.Enabled = True

        ' ...change text caption back to Send
        sendBtn.Text = "Push"

        ' set progressbar position back to beginning - just for show, doesn't really matter
        progressBar.Value = 0

        ' set copy panel to nothing to make it invisible again - it's auto-resizing :)
        copyPanel.Text = ""

        ' inform the user what is happening
        If m_boolAborted Then

            ' ...if transfer was aborted..
            informLabel.Text = "Push aborted. Click on Push to select file(s) to push to device."

        ElseIf (m_boolDenied) Then

            ' .. if push was denied by receiving device
            informLabel.Text = "Push denied by device. Click on Push to select file(s) to push to device."

        ElseIf (m_boolUnrecoverableError) Then

            ' ..if stream was lost
            informLabel.Text = "Stream lost to device. Click on the device you wish to push file(s) to."

        Else

            ' ...transfer completed
            informLabel.Text = "File(s) sent to device. Click on Push to select file(s) to push to device."
        End If

        ' enable device list again when finished
        deviceList.Enabled = True

        ' we enable the Send button again if it was disabled when it was a Cancel button
        sendBtn.Enabled = True

    End Sub

    Private Sub m_objectPush_PutFileBegin(ByVal sender As Object, ByVal eventArgs As ObexEventArgs) Handles m_objectPush.PutFileBegin
        ' this transfer hasn't been aborted (yet!)
        m_boolAborted = False

        'this transfer hasn't been denied (yet!)
        m_boolDenied = False

        ' this transfer has not had any errors (yet!)
        m_boolUnrecoverableError = False

        ' while copying, disable the discover button
        discoverBtn.Enabled = False

        sendBtn.Text = "Cancel"

        ' disable device list while copying
        deviceList.Enabled = False

        ' inform the user what is happening
        informLabel.Text = "Sending file(s) to device..."

    End Sub

    Private Sub m_objectPush_Error(ByVal sender As Object, ByVal eventArgs As ObexEventArgs) Handles m_objectPush.Error

        Dim errorArgs As ObexErrorEventArgs = DirectCast(eventArgs, ObexErrorEventArgs)

        ' if the error was not an Obex error - we can assume the Stream is not available anymore
        If errorArgs.Error = Franson.Protocols.Obex.ErrorCode.StreamError Then

            ' the Push button
            sendBtn.Enabled = False

            ' the device list
            deviceList.Enabled = True

            ' this error can't be recovered from
            m_boolUnrecoverableError = True

            ' show this error in a box - it's pretty serious
            MessageBox.Show(errorArgs.Message)

        Else

            ' if the error is transfer was aborted, set boolean so we can display this to user
            If errorArgs.Error = Franson.Protocols.Obex.ErrorCode.Aborted Then

                m_boolAborted = True

            End If

            If errorArgs.Error = Franson.Protocols.Obex.ErrorCode.SecurityAbort Then

                m_boolAborted = True

            End If

            If errorArgs.Error = Franson.Protocols.Obex.ErrorCode.SendDenied Then

                m_boolDenied = True
            End If

            ' show message to user
            MessageBox.Show(errorArgs.Message)
            End If
    End Sub

    Private Sub m_deviceCurrent_Error(ByVal sender As Object, ByVal eventArgs As BlueToolsEventArgs) Handles m_deviceCurrent.Error

        Dim errorArgs As Franson.BlueTools.ErrorEventArgs = DirectCast(eventArgs, Franson.BlueTools.ErrorEventArgs)

        statusBar.Text = errorArgs.Message

    End Sub

    Private Sub m_objectPush_DisconnectEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles m_objectPush.DisconnectEnd

        ' we close this stream to ensure that we get a fresh new one when pushing next time.
        ' this is needed since some devices do not allow multiple pushes in one stream
        If Not m_streamCurrent Is Nothing Then

            m_intTransfers = m_intTransfers - 1

            If m_intTransfers = 0 Then

                m_streamCurrent.Close()

            End If


        End If

    End Sub
End Class
