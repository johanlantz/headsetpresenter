Imports Franson.BlueTools
Imports Franson.Protocols.Obex
Imports Franson.Protocols.Obex.FTPClient
Imports System.IO

Public Class MainForm
    Inherits System.Windows.Forms.Form
    Friend WithEvents saveFileDialog As System.Windows.Forms.SaveFileDialog
    Friend WithEvents openFileDialog As System.Windows.Forms.OpenFileDialog
    Friend WithEvents tabControl As System.Windows.Forms.TabControl
    Friend WithEvents deviceTab As System.Windows.Forms.TabPage
    Friend WithEvents deviceHelpBtn As System.Windows.Forms.Button
    Friend WithEvents connectBtn As System.Windows.Forms.Button
    Friend WithEvents disconnectBtn As System.Windows.Forms.Button
    Friend WithEvents discoverBtn As System.Windows.Forms.Button
    Friend WithEvents deviceList As System.Windows.Forms.ListBox
    Friend WithEvents fileTab As System.Windows.Forms.TabPage
    Friend WithEvents allowSelectCheckBox As System.Windows.Forms.CheckBox
    Friend WithEvents helpBtn As System.Windows.Forms.Button
    Friend WithEvents downloadBtn As System.Windows.Forms.Button
    Friend WithEvents uploadBtn As System.Windows.Forms.Button
    Friend WithEvents deleteBtn As System.Windows.Forms.Button
    Friend WithEvents createBtn As System.Windows.Forms.Button
    Friend WithEvents progressBar As System.Windows.Forms.ProgressBar
    Friend WithEvents fileList As System.Windows.Forms.ListBox

    Dim m_manager As Manager = Nothing
    Dim WithEvents m_network As Network = Nothing
    Dim m_serviceCurrent As RemoteService = Nothing
    Dim WithEvents m_deviceCurrent As RemoteDevice = Nothing
    Dim m_streamCurrent As Stream = Nothing
    Dim WithEvents fileBrowser As fileBrowser = Nothing

    ' count errors
    Dim m_iUnrecoverableError As Int16 = 0

    ' determine what to happened on DisconnectEnd
    Dim m_boolShuttingDown As Boolean = False

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        Try
            Dim license As Franson.BlueTools.License = New Franson.BlueTools.License
            license.LicenseKey = "HU5UZer022JOLX98oidhQkbRbvv0NRJbHXEc"

            ' get bluetools manager
            m_manager = Manager.GetManager()

            ' Call events in GUI thread
            m_manager.Parent = Me

            ' Get first netowrk (BlueTools 1.0 only supports one network == one dongle)
            m_network = m_manager.Networks(0)

        Catch exc As Exception

            MessageBox.Show(exc.Message)
        End Try
        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)

        MyBase.Dispose(disposing)
    End Sub

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Private Sub InitializeComponent()
        Me.saveFileDialog = New System.Windows.Forms.SaveFileDialog
        Me.openFileDialog = New System.Windows.Forms.OpenFileDialog
        Me.tabControl = New System.Windows.Forms.TabControl
        Me.deviceTab = New System.Windows.Forms.TabPage
        Me.deviceHelpBtn = New System.Windows.Forms.Button
        Me.connectBtn = New System.Windows.Forms.Button
        Me.disconnectBtn = New System.Windows.Forms.Button
        Me.discoverBtn = New System.Windows.Forms.Button
        Me.deviceList = New System.Windows.Forms.ListBox
        Me.fileTab = New System.Windows.Forms.TabPage
        Me.allowSelectCheckBox = New System.Windows.Forms.CheckBox
        Me.helpBtn = New System.Windows.Forms.Button
        Me.downloadBtn = New System.Windows.Forms.Button
        Me.uploadBtn = New System.Windows.Forms.Button
        Me.deleteBtn = New System.Windows.Forms.Button
        Me.createBtn = New System.Windows.Forms.Button
        Me.progressBar = New System.Windows.Forms.ProgressBar
        Me.fileList = New System.Windows.Forms.ListBox
        Me.tabControl.SuspendLayout()
        Me.deviceTab.SuspendLayout()
        Me.fileTab.SuspendLayout()
        Me.SuspendLayout()
        '
        'saveFileDialog
        '
        Me.saveFileDialog.FileName = "doc1"
        '
        'tabControl
        '
        Me.tabControl.Controls.Add(Me.deviceTab)
        Me.tabControl.Controls.Add(Me.fileTab)
        Me.tabControl.Location = New System.Drawing.Point(0, 0)
        Me.tabControl.Name = "tabControl"
        Me.tabControl.SelectedIndex = 0
        Me.tabControl.Size = New System.Drawing.Size(240, 248)
        Me.tabControl.TabIndex = 0
        '
        'deviceTab
        '
        Me.deviceTab.Controls.Add(Me.deviceHelpBtn)
        Me.deviceTab.Controls.Add(Me.connectBtn)
        Me.deviceTab.Controls.Add(Me.disconnectBtn)
        Me.deviceTab.Controls.Add(Me.discoverBtn)
        Me.deviceTab.Controls.Add(Me.deviceList)
        Me.deviceTab.Location = New System.Drawing.Point(0, 0)
        Me.deviceTab.Name = "deviceTab"
        Me.deviceTab.Size = New System.Drawing.Size(240, 225)
        Me.deviceTab.Text = "Devices"
        '
        'deviceHelpBtn
        '
        Me.deviceHelpBtn.Location = New System.Drawing.Point(72, 192)
        Me.deviceHelpBtn.Name = "deviceHelpBtn"
        Me.deviceHelpBtn.Size = New System.Drawing.Size(72, 20)
        Me.deviceHelpBtn.TabIndex = 0
        Me.deviceHelpBtn.Text = "Help"
        '
        'connectBtn
        '
        Me.connectBtn.Enabled = False
        Me.connectBtn.Location = New System.Drawing.Point(56, 164)
        Me.connectBtn.Name = "connectBtn"
        Me.connectBtn.Size = New System.Drawing.Size(72, 20)
        Me.connectBtn.TabIndex = 1
        Me.connectBtn.Text = "Connect"
        '
        'disconnectBtn
        '
        Me.disconnectBtn.Enabled = False
        Me.disconnectBtn.Location = New System.Drawing.Point(136, 164)
        Me.disconnectBtn.Name = "disconnectBtn"
        Me.disconnectBtn.Size = New System.Drawing.Size(72, 20)
        Me.disconnectBtn.TabIndex = 2
        Me.disconnectBtn.Text = "Disconnect"
        '
        'discoverBtn
        '
        Me.discoverBtn.Location = New System.Drawing.Point(8, 164)
        Me.discoverBtn.Name = "discoverBtn"
        Me.discoverBtn.Size = New System.Drawing.Size(40, 20)
        Me.discoverBtn.TabIndex = 3
        Me.discoverBtn.Text = "Scan"
        '
        'deviceList
        '
        Me.deviceList.Location = New System.Drawing.Point(8, 8)
        Me.deviceList.Name = "deviceList"
        Me.deviceList.Size = New System.Drawing.Size(216, 142)
        Me.deviceList.TabIndex = 4
        '
        'fileTab
        '
        Me.fileTab.Controls.Add(Me.allowSelectCheckBox)
        Me.fileTab.Controls.Add(Me.helpBtn)
        Me.fileTab.Controls.Add(Me.downloadBtn)
        Me.fileTab.Controls.Add(Me.uploadBtn)
        Me.fileTab.Controls.Add(Me.deleteBtn)
        Me.fileTab.Controls.Add(Me.createBtn)
        Me.fileTab.Controls.Add(Me.progressBar)
        Me.fileTab.Controls.Add(Me.fileList)
        Me.fileTab.Location = New System.Drawing.Point(0, 0)
        Me.fileTab.Name = "fileTab"
        Me.fileTab.Size = New System.Drawing.Size(232, 222)
        Me.fileTab.Text = "Files"
        '
        'allowSelectCheckBox
        '
        Me.allowSelectCheckBox.Location = New System.Drawing.Point(104, 192)
        Me.allowSelectCheckBox.Name = "allowSelectCheckBox"
        Me.allowSelectCheckBox.Size = New System.Drawing.Size(112, 20)
        Me.allowSelectCheckBox.TabIndex = 0
        Me.allowSelectCheckBox.Text = "Allow folder select"
        '
        'helpBtn
        '
        Me.helpBtn.Location = New System.Drawing.Point(16, 192)
        Me.helpBtn.Name = "helpBtn"
        Me.helpBtn.Size = New System.Drawing.Size(72, 20)
        Me.helpBtn.TabIndex = 1
        Me.helpBtn.Text = "Help"
        '
        'downloadBtn
        '
        Me.downloadBtn.Enabled = False
        Me.downloadBtn.Location = New System.Drawing.Point(128, 136)
        Me.downloadBtn.Name = "downloadBtn"
        Me.downloadBtn.Size = New System.Drawing.Size(88, 20)
        Me.downloadBtn.TabIndex = 2
        Me.downloadBtn.Text = "Download"
        '
        'uploadBtn
        '
        Me.uploadBtn.Enabled = False
        Me.uploadBtn.Location = New System.Drawing.Point(16, 136)
        Me.uploadBtn.Name = "uploadBtn"
        Me.uploadBtn.Size = New System.Drawing.Size(88, 20)
        Me.uploadBtn.TabIndex = 3
        Me.uploadBtn.Text = "Upload"
        '
        'deleteBtn
        '
        Me.deleteBtn.Enabled = False
        Me.deleteBtn.Location = New System.Drawing.Point(128, 160)
        Me.deleteBtn.Name = "deleteBtn"
        Me.deleteBtn.Size = New System.Drawing.Size(88, 20)
        Me.deleteBtn.TabIndex = 4
        Me.deleteBtn.Text = "Delete"
        '
        'createBtn
        '
        Me.createBtn.Enabled = False
        Me.createBtn.Location = New System.Drawing.Point(16, 160)
        Me.createBtn.Name = "createBtn"
        Me.createBtn.Size = New System.Drawing.Size(88, 20)
        Me.createBtn.TabIndex = 5
        Me.createBtn.Text = "Create dir"
        '
        'progressBar
        '
        Me.progressBar.Location = New System.Drawing.Point(8, 120)
        Me.progressBar.Name = "progressBar"
        Me.progressBar.Size = New System.Drawing.Size(214, 8)
        '
        'fileList
        '
        Me.fileList.Location = New System.Drawing.Point(8, 8)
        Me.fileList.Name = "fileList"
        Me.fileList.Size = New System.Drawing.Size(216, 100)
        Me.fileList.TabIndex = 7
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(96.0!, 96.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi
        Me.ClientSize = New System.Drawing.Size(240, 294)
        Me.Controls.Add(Me.tabControl)
        Me.MinimizeBox = False
        Me.Name = "MainForm"
        Me.Text = "ObexFTPClient Sample"
        Me.tabControl.ResumeLayout(False)
        Me.deviceTab.ResumeLayout(False)
        Me.fileTab.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub m_network_DeviceDiscovered(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_network.DeviceDiscovered

        Dim discoveryEventArgs As DiscoveryEventArgs = DirectCast(eventArgs, DiscoveryEventArgs)
        Dim device As Device = DirectCast(discoveryEventArgs.Discovery, Device)

        deviceList.Items.Add(device)

    End Sub

    Private Sub m_network_DeviceDiscoveryStarted(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_network.DeviceDiscoveryStarted
        ' empty the control
        deviceList.Items.Clear()

        For Each device As RemoteDevice In m_network.Devices

            deviceList.Items.Add(device)
        Next

        ' disable the button - can only run one device discovery at the time after all
        discoverBtn.Enabled = False

        ' disable the listbox
        deviceList.Enabled = False

    End Sub

    Private Sub m_network_DeviceDiscoveryCompleted(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_network.DeviceDiscoveryCompleted
        ' enable the button - user might want to scan devices again!
        discoverBtn.Enabled = True

        ' enable the listbox so we can connect to devices.
        deviceList.Enabled = True

    End Sub

    Private Sub discoverBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles discoverBtn.Click
        If Not m_network.DiscoveryPending Then
            ' find devices in the vicinity
            m_network.DiscoverDevicesAsync()
        End If

    End Sub

    Private Sub deviceList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles deviceList.SelectedIndexChanged

        connectBtn.Enabled = True

    End Sub

    Private Sub m_deviceCurrent_ServiceDiscoveryCompleted(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_deviceCurrent.ServiceDiscoveryCompleted

        Dim discoveryArgs As DiscoveryEventArgs = DirectCast(eventArgs, DiscoveryEventArgs)

        Dim services As Service() = DirectCast(discoveryArgs.Discovery, Service())

        ' if services is above 0 we found an Obex FTP service.
        If services.Length > 0 Then

            Try

                Dim remoteService As RemoteService = DirectCast(services(0), RemoteService)
                m_streamCurrent = remoteService.Stream

                fileBrowser = New FileBrowser

                ' marshal events into this class thread
                fileBrowser.Parent = Me


                fileBrowser.ConnectAsync(m_streamCurrent)
                fileBrowser.GetPathAsync()

            Catch exc As Exception

                ' if connection fails we must handle this too and restore access to ui

                discoverBtn.Enabled = True
                deviceList.Enabled = True

                MessageBox.Show(exc.Message)
            End Try


        Else

            ' if there was no Obex FTP service found we must enable parts of the UI again so user
            ' can select another device
            deviceList.Enabled = True
            discoverBtn.Enabled = True

            MessageBox.Show("No Obex FTP service found on selected device.")

        End If

    End Sub

    Private Sub downloadBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles downloadBtn.Click

        If downloadBtn.Text.Equals("Abort") Then

            fileBrowser.Abort()

        Else

            ' avoid the .. item - it has no FileFolder
            If fileList.SelectedIndex > 0 Then

                ' get the selected file from the cache
                Dim ff As FileFolder = fileBrowser.Items(fileList.SelectedIndex - 1)

                saveFileDialog.FileName = ff.Name

                ' this must be added so CF will add the extension properly.
                saveFileDialog.Filter = Path.GetExtension(ff.Name) + "-files|" + Path.GetExtension(ff.Name)

                If (saveFileDialog.ShowDialog() = Windows.Forms.DialogResult.OK) Then

                    downloadBtn.Text = "Abort"

                    Dim fs As FileStream = New FileStream(saveFileDialog.FileName, FileMode.Create, FileAccess.Write)
                    fileBrowser.GetFileAsync(fs, ff.Name)

                End If
            End If
        End If
    End Sub

    Private Sub helpBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles helpBtn.Click

        ' show help how to use FTP
        Dim text As String = "Click on Upload or Download to transfer the selected file. (Only works on files.)" & vbCrLf & vbCrLf
        text += "Click on Create dir to create a new sub-directory." & vbCrLf & vbCrLf
        text += "Click Delete to delete the selected file or folder." & vbCrLf & vbCrLf
        text += "To Browse the FTP, click on an item marked ""Folder:"". A click on "".."" steps back to the parent directory."
        text += "To delete a folder, click ""Allow folder select"" to avoid stepping into it."

        MessageBox.Show(text)

    End Sub

    Private Sub deviceHelpBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles deviceHelpBtn.Click
        ' show short manual how to use this client

        Dim text As String
        text = "Click on Scan to detect nearby Bluetooth devices." & vbCrLf & vbCrLf
        text += "Select a device from the list and Click on Connect to connect to the selected device." & vbCrLf & vbCrLf
        text += "Click on disconnect to disconnect from the remote device."

        MessageBox.Show(text)

    End Sub

    Private Sub connectBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles connectBtn.Click

        m_deviceCurrent = DirectCast(deviceList.SelectedItem, RemoteDevice)

        m_deviceCurrent.DiscoverServicesAsync(ServiceType.OBEXFileTransfer)

        discoverBtn.Enabled = False
        connectBtn.Enabled = False
        deviceList.Enabled = False

    End Sub

    Private Sub disconnectBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles disconnectBtn.Click

        ' click disconnect we can attempt to disconnect
        fileBrowser.DisconnectAsync()

    End Sub

    Private Sub uploadBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles uploadBtn.Click

        If uploadBtn.Text.Equals("Abort") Then

            fileBrowser.Abort()

        Else

            If openFileDialog.ShowDialog() = Windows.Forms.DialogResult.OK Then

                uploadBtn.Text = "Abort"

                Dim fs As FileStream = New FileStream(openFileDialog.FileName, FileMode.Open, FileAccess.Read)
                fileBrowser.PutFileAsync(fs, Path.GetFileName(openFileDialog.FileName))

            End If
        End If
    End Sub

    Private Sub createBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles createBtn.Click

        fileList.Enabled = False
        fileBrowser.CreateDirectoryAsync("Test directory")

    End Sub

    Private Sub deleteBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles deleteBtn.Click

        ' if we click on any other than the first
        If fileList.SelectedIndex > 0 Then

            ' get the selected file from the cache
            Dim filename As String = fileBrowser.Items(fileList.SelectedIndex - 1).Name

            fileBrowser.DeleteAsync(filename)
            fileList.Enabled = False
            uploadBtn.Enabled = False
            downloadBtn.Enabled = False
            deleteBtn.Enabled = False
            createBtn.Enabled = False
        End If

    End Sub

    Private Sub fileList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles fileList.SelectedIndexChanged

        ' if we don't allow selection of folders
        If Not allowSelectCheckBox.Checked Then

            ' enable download button when we have a selected file
            downloadBtn.Enabled = Not fileList.SelectedIndex = 0
            ' enable delete button when we have a selected file
            deleteBtn.Enabled = Not fileList.SelectedIndex = 0

            ' if we click on the top, it's .. (back one step)
            If fileList.SelectedIndex = 0 Then

                fileList.Enabled = False

                ' tell file browser to step back one directory
                fileBrowser.SetPathAsync("..")
            End If
            ' if we click on any other
        ElseIf fileList.SelectedIndex > 0 Then

            ' get FileFolder object from cache
            Dim ff As FileFolder = fileBrowser.Items(fileList.SelectedIndex - 1)

            ' check if it's a folder
            If Not ff Is Nothing Then

                If ff.IsFolder Then
                    fileList.Enabled = False

                    fileBrowser.SetPathAsync(ff.Name)
                End If
            End If
        End If
    End Sub

    Private Sub MainForm_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing

        ' if this boolean is set we have done all this already
        If m_boolShuttingDown Then
            Return
        End If

        If Not fileBrowser Is Nothing Then
            If fileBrowser.Connected Then

                fileBrowser.DisconnectAsync()

                m_boolShuttingDown = True
                e.Cancel = True

            End If
        Else

            If Not m_network Is Nothing Then

                ' cancel device discovery if we're closing down
                m_network.CancelDeviceDiscovery()

            End If

            If Not m_manager Is Nothing Then

                m_manager.Dispose()

            End If
        End If

    End Sub

    
    Private Sub fileBrowser_DisconnectEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.DisconnectEnd

        If Not m_boolShuttingDown Then

            ' we disconnected, so we can change target device
            deviceList.Enabled = True
            fileList.Enabled = False

            ' empty file list
            fileList.Items.Clear()

            ' we're not connected, we can't disconnect
            disconnectBtn.Enabled = False

            ' can't access files if we're not connected
            uploadBtn.Enabled = False
            downloadBtn.Enabled = False
            deleteBtn.Enabled = False
            createBtn.Enabled = False
            discoverBtn.Enabled = True

            If Not m_streamCurrent Is Nothing Then

                m_streamCurrent.Close()
                m_streamCurrent = Nothing

                m_serviceCurrent = Nothing

            End If
        Else
            If Not m_network Is Nothing Then

                m_network.CancelDeviceDiscovery()
            End If

            If Not m_manager Is Nothing Then
                ' must be disposed!
                m_manager.Dispose()
            End If

            ' close the form
            Close()

        End If
    End Sub

    Private Sub fileBrowser_ConnectEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.ConnectEnd

        ' there has been no errors (yet!)
        m_iUnrecoverableError = 0

        ' disable the deviceList while we're connected
        deviceList.Enabled = False

        ' we can attempt to disconnect
        disconnectBtn.Enabled = True

        ' can't scan when we're connected
        discoverBtn.Enabled = False

        ' change to tab page with files
        tabControl.SelectedIndex = 1

    End Sub

    Private Sub fileBrowser_CreateDirectoryEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.CreateDirectoryEnd

        If m_iUnrecoverableError = 0 Then

            ' when new directory is created, update file list box
            fileBrowser.GetPathAsync()
        End If


    End Sub

    Private Sub fileBrowser_DeleteEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.DeleteEnd
        ' update file list box
        fileBrowser.GetPathAsync()
    End Sub

    Private Sub fileBrowser_Error(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.Error

        Dim errorArgs As ObexErrorEventArgs = DirectCast(e, ObexErrorEventArgs)

        If errorArgs.Error = Franson.Protocols.Obex.ErrorCode.StreamError Then

            m_iUnrecoverableError = m_iUnrecoverableError + 1
        End If

        If errorArgs.Error = Franson.Protocols.Obex.ErrorCode.Aborted Then

            MessageBox.Show("Copy was aborted")

        ElseIf errorArgs.Error = Franson.Protocols.Obex.ErrorCode.SecurityAbort Then

            MessageBox.Show("Copy was aborted for security reasons")

            ' a security abort should expect the client to disconnect. so we do that and restore the connection again
            fileBrowser.DisconnectAsync()
            fileBrowser.ConnectAsync(m_streamCurrent)

        Else

            MessageBox.Show(errorArgs.Message)
        End If

        If m_iUnrecoverableError = 1 Then

            DisableUI()
        End If


    End Sub

    Private Sub fileBrowser_GetPathEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.GetPathEnd

        fileList.Items.Clear()

        Dim contents As FileFolder() = fileBrowser.Items.ToArray()

        For Each ff As FileFolder In contents

            If (ff.IsFolder) Then

                fileList.Items.Add("Folder: " + ff.Name)

            Else

                fileList.Items.Add("File: " + ff.Name)

            End If
        Next

        fileList.Items.Insert(0, "..")

        ' enable the list box again
        fileList.Enabled = True
        downloadBtn.Enabled = True
        uploadBtn.Enabled = True
        deleteBtn.Enabled = True
        createBtn.Enabled = True
    End Sub

    Private Sub fileBrowser_PutFileEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.PutFileEnd

        Dim copyArgs As ObexCopyEventArgs = DirectCast(e, ObexCopyEventArgs)

        ' close the stream we opened before
        copyArgs.Stream.Close()


        ' return progressBar to beginning
        progressBar.Value = 0

        fileList.Enabled = True

        ' restore button captions
        uploadBtn.Text = "Upload"
        downloadBtn.Text = "Download"

        If m_iUnrecoverableError = 0 Then

            ' enable buttons
            uploadBtn.Enabled = True
            downloadBtn.Enabled = True
            ' update current folder
            fileBrowser.GetPathAsync()
        End If

    End Sub

    Private Sub fileBrowser_PutFileProgress(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.PutFileProgress

        Dim copyArgs As ObexCopyEventArgs = DirectCast(e, ObexCopyEventArgs)

        If Not copyArgs.Size = -1 Then

            If Not copyArgs.Size = progressBar.Maximum Then

                progressBar.Maximum = copyArgs.Size
            End If

            progressBar.Value = copyArgs.Position
        End If

        fileList.Enabled = False

    End Sub

    Private Sub fileBrowser_SetPathEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.SetPathEnd

        If m_iUnrecoverableError = 0 Then

            fileBrowser.GetPathAsync()

        End If

    End Sub

    Private Sub DisableUI()

        ' disable buttons
        disconnectBtn.Enabled = False
        uploadBtn.Enabled = False
        downloadBtn.Enabled = False
        deleteBtn.Enabled = False
        createBtn.Enabled = False

        ' re-enable scan (if it was scan button was disabled)
        discoverBtn.Enabled = True

        ' clear controls since we can't access the contents anymore
        fileList.Items.Clear()

        ' enable device list again - we must able to pick a new device if we want to
        deviceList.Enabled = True

        ' change to device tab page
        tabControl.SelectedIndex = 0
    End Sub

    Private Sub fileBrowser_GetFileProgress(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.GetFileProgress

        fileBrowser_PutFileProgress(sender, e)

    End Sub

    Private Sub fileBrowser_GetFileEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fileBrowser.GetFileEnd

        fileBrowser_PutFileEnd(sender, e)

    End Sub
End Class
