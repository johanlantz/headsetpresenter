Imports Franson.BlueTools
Imports Franson.Protocols.Obex
Imports Franson.Protocols.Obex.FTPClient
Imports System.IO

Public Class MainForm
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        Try
            m_manager = Manager.GetManager()

            ' show which stack application is using
            Select Case Manager.StackID

            Case StackID.STACK_MICROSOFT

                    stackPanel.Text = "Microsoft Bluetooth Stack"

                Case StackID.STACK_WIDCOMM

                    stackPanel.Text = "WidComm Bluetooth Stack"
            End Select

            ' marshal events in this class thread
            m_manager.Parent = Me

            ' get your trial license from franson.com or buy BlueTools
            Dim License As Franson.BlueTools.License = New Franson.BlueTools.License
            License.LicenseKey = "WoK6IL448AACKORQWXWZTjvRHRfuLYDXWsK6"

            ' Get first network (BlueTools 1.0 only supports one network == one dongle)
            m_network = m_manager.Networks(0)

            ' load address book
            m_addressbook = New AddressBook
            m_addressbook.Load()

            ' create FileBrowser object
            fb = New FileBrowser

            ' marshal events to this class thread.
            ' this is necessary if you for example (like this sample does) want to display the progress
            ' of a copy operation with a progress bar. without this marshalling you will end up calling
            ' the event method from an internal FileBrowser thread. This can cause a lot of problems
            ' with synchronization and deadlocks. By passing this class thread to FileBrowser it will
            ' invoke the events in this thread and thus not cause the problems stated above.
            ' If you want to know more read up on Invoke and BeginInvoke.
            fb.Parent = Me

            ListDevices()

        Catch exc As Exception

            MessageBox.Show(exc.Message)
        End Try

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then

            ' must dispose manager, else application can't exit
            If Not m_manager Is Nothing Then

                m_manager.Dispose()

            End If

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
    Friend WithEvents statusBarPanel As System.Windows.Forms.StatusBarPanel
    Friend WithEvents SaveFileDialog As System.Windows.Forms.SaveFileDialog
    Friend WithEvents OpenFileDialog As System.Windows.Forms.OpenFileDialog
    Friend WithEvents ProgressBar As System.Windows.Forms.ProgressBar
    Friend WithEvents StatusBar As System.Windows.Forms.StatusBar
    Friend WithEvents stackPanel As System.Windows.Forms.StatusBarPanel
    Friend WithEvents treeImageList As System.Windows.Forms.ImageList
    Friend WithEvents toolbarImageList As System.Windows.Forms.ImageList
    Friend WithEvents discoverBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents disconnectBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents refreshBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents createDirBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents delBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents putBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents getBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents abortBtn As System.Windows.Forms.ToolBarButton
    Friend WithEvents folderTree As System.Windows.Forms.TreeView
    Friend WithEvents fileListView As System.Windows.Forms.ListView
    Friend WithEvents NameColumn As System.Windows.Forms.ColumnHeader
    Friend WithEvents SizeColumn As System.Windows.Forms.ColumnHeader
    Friend WithEvents ModificationColumn As System.Windows.Forms.ColumnHeader
    Friend WithEvents Splitter As System.Windows.Forms.Splitter
    Friend WithEvents toolBar As System.Windows.Forms.ToolBar
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(MainForm))
        Me.StatusBar = New System.Windows.Forms.StatusBar
        Me.statusBarPanel = New System.Windows.Forms.StatusBarPanel
        Me.stackPanel = New System.Windows.Forms.StatusBarPanel
        Me.SaveFileDialog = New System.Windows.Forms.SaveFileDialog
        Me.OpenFileDialog = New System.Windows.Forms.OpenFileDialog
        Me.ProgressBar = New System.Windows.Forms.ProgressBar
        Me.treeImageList = New System.Windows.Forms.ImageList(Me.components)
        Me.toolbarImageList = New System.Windows.Forms.ImageList(Me.components)
        Me.toolBar = New System.Windows.Forms.ToolBar
        Me.discoverBtn = New System.Windows.Forms.ToolBarButton
        Me.disconnectBtn = New System.Windows.Forms.ToolBarButton
        Me.refreshBtn = New System.Windows.Forms.ToolBarButton
        Me.createDirBtn = New System.Windows.Forms.ToolBarButton
        Me.delBtn = New System.Windows.Forms.ToolBarButton
        Me.putBtn = New System.Windows.Forms.ToolBarButton
        Me.getBtn = New System.Windows.Forms.ToolBarButton
        Me.abortBtn = New System.Windows.Forms.ToolBarButton
        Me.folderTree = New System.Windows.Forms.TreeView
        Me.fileListView = New System.Windows.Forms.ListView
        Me.NameColumn = New System.Windows.Forms.ColumnHeader
        Me.SizeColumn = New System.Windows.Forms.ColumnHeader
        Me.ModificationColumn = New System.Windows.Forms.ColumnHeader
        Me.Splitter = New System.Windows.Forms.Splitter
        CType(Me.statusBarPanel, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.stackPanel, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'StatusBar
        '
        Me.StatusBar.Location = New System.Drawing.Point(0, 463)
        Me.StatusBar.Name = "StatusBar"
        Me.StatusBar.Panels.AddRange(New System.Windows.Forms.StatusBarPanel() {Me.statusBarPanel, Me.stackPanel})
        Me.StatusBar.ShowPanels = True
        Me.StatusBar.Size = New System.Drawing.Size(792, 22)
        Me.StatusBar.TabIndex = 1
        Me.StatusBar.Text = "statusBar"
        '
        'statusBarPanel
        '
        Me.statusBarPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring
        Me.statusBarPanel.Width = 766
        '
        'stackPanel
        '
        Me.stackPanel.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents
        Me.stackPanel.Width = 10
        '
        'ProgressBar
        '
        Me.ProgressBar.Dock = System.Windows.Forms.DockStyle.Bottom
        Me.ProgressBar.Location = New System.Drawing.Point(0, 455)
        Me.ProgressBar.Name = "ProgressBar"
        Me.ProgressBar.Size = New System.Drawing.Size(792, 8)
        Me.ProgressBar.TabIndex = 5
        Me.ProgressBar.Visible = False
        '
        'treeImageList
        '
        Me.treeImageList.ImageSize = New System.Drawing.Size(16, 16)
        Me.treeImageList.ImageStream = CType(resources.GetObject("treeImageList.ImageStream"), System.Windows.Forms.ImageListStreamer)
        Me.treeImageList.TransparentColor = System.Drawing.Color.Transparent
        '
        'toolbarImageList
        '
        Me.toolbarImageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit
        Me.toolbarImageList.ImageSize = New System.Drawing.Size(32, 32)
        Me.toolbarImageList.ImageStream = CType(resources.GetObject("toolbarImageList.ImageStream"), System.Windows.Forms.ImageListStreamer)
        Me.toolbarImageList.TransparentColor = System.Drawing.Color.Transparent
        '
        'toolBar
        '
        Me.toolBar.Appearance = System.Windows.Forms.ToolBarAppearance.Flat
        Me.toolBar.Buttons.AddRange(New System.Windows.Forms.ToolBarButton() {Me.discoverBtn, Me.disconnectBtn, Me.refreshBtn, Me.createDirBtn, Me.delBtn, Me.putBtn, Me.getBtn, Me.abortBtn})
        Me.toolBar.DropDownArrows = True
        Me.toolBar.ImageList = Me.toolbarImageList
        Me.toolBar.Location = New System.Drawing.Point(0, 0)
        Me.toolBar.Name = "toolBar"
        Me.toolBar.ShowToolTips = True
        Me.toolBar.Size = New System.Drawing.Size(792, 44)
        Me.toolBar.TabIndex = 6
        '
        'discoverBtn
        '
        Me.discoverBtn.ImageIndex = 0
        Me.discoverBtn.ToolTipText = "Scan for nearby Bluetooth devices."
        '
        'disconnectBtn
        '
        Me.disconnectBtn.Enabled = False
        Me.disconnectBtn.ImageIndex = 1
        Me.disconnectBtn.ToolTipText = "Disconnect from Obex server."
        '
        'refreshBtn
        '
        Me.refreshBtn.Enabled = False
        Me.refreshBtn.ImageIndex = 5
        Me.refreshBtn.ToolTipText = "Refresh"
        '
        'createDirBtn
        '
        Me.createDirBtn.Enabled = False
        Me.createDirBtn.ImageIndex = 6
        Me.createDirBtn.ToolTipText = "Create new directory"
        '
        'delBtn
        '
        Me.delBtn.Enabled = False
        Me.delBtn.ImageIndex = 4
        Me.delBtn.ToolTipText = "Delete selected file or folder."
        '
        'putBtn
        '
        Me.putBtn.Enabled = False
        Me.putBtn.ImageIndex = 3
        Me.putBtn.ToolTipText = "Upload selected file(s)."
        '
        'getBtn
        '
        Me.getBtn.Enabled = False
        Me.getBtn.ImageIndex = 7
        Me.getBtn.ToolTipText = "Download selected file(s)."
        '
        'abortBtn
        '
        Me.abortBtn.Enabled = False
        Me.abortBtn.ImageIndex = 2
        Me.abortBtn.ToolTipText = "Abort copy."
        '
        'folderTree
        '
        Me.folderTree.Dock = System.Windows.Forms.DockStyle.Left
        Me.folderTree.ImageList = Me.treeImageList
        Me.folderTree.Location = New System.Drawing.Point(0, 44)
        Me.folderTree.Name = "folderTree"
        Me.folderTree.Size = New System.Drawing.Size(232, 411)
        Me.folderTree.TabIndex = 7
        '
        'fileListView
        '
        Me.fileListView.Activation = System.Windows.Forms.ItemActivation.OneClick
        Me.fileListView.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.NameColumn, Me.SizeColumn, Me.ModificationColumn})
        Me.fileListView.Dock = System.Windows.Forms.DockStyle.Fill
        Me.fileListView.FullRowSelect = True
        Me.fileListView.Location = New System.Drawing.Point(232, 44)
        Me.fileListView.Name = "fileListView"
        Me.fileListView.Size = New System.Drawing.Size(560, 411)
        Me.fileListView.SmallImageList = Me.treeImageList
        Me.fileListView.TabIndex = 8
        Me.fileListView.View = System.Windows.Forms.View.Details
        '
        'NameColumn
        '
        Me.NameColumn.Text = "Name"
        Me.NameColumn.Width = 250
        '
        'SizeColumn
        '
        Me.SizeColumn.Text = "Size"
        Me.SizeColumn.Width = 80
        '
        'ModificationColumn
        '
        Me.ModificationColumn.Text = "Date modified"
        Me.ModificationColumn.Width = 80
        '
        'Splitter
        '
        Me.Splitter.Location = New System.Drawing.Point(232, 44)
        Me.Splitter.Name = "Splitter"
        Me.Splitter.Size = New System.Drawing.Size(3, 411)
        Me.Splitter.TabIndex = 9
        Me.Splitter.TabStop = False
        '
        'MainForm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(792, 485)
        Me.Controls.Add(Me.Splitter)
        Me.Controls.Add(Me.fileListView)
        Me.Controls.Add(Me.folderTree)
        Me.Controls.Add(Me.toolBar)
        Me.Controls.Add(Me.ProgressBar)
        Me.Controls.Add(Me.StatusBar)
        Me.Name = "MainForm"
        Me.Text = "MainForm"
        CType(Me.statusBarPanel, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.stackPanel, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

#End Region

    ' - User defined variables -

    ' Application
    Dim m_addressbook As AddressBook = Nothing
    Dim WithEvents fb As FileBrowser = Nothing

    ' BlueTools
    Dim m_manager As Manager = Nothing
    Dim WithEvents m_network As Network = Nothing
    Dim m_serviceCurrent As RemoteService = Nothing
    Dim WithEvents m_deviceCurrent As RemoteDevice = Nothing

    Dim m_streamCurrent As Stream = Nothing

    ' stores how many unrecoverable error has occurred
    Dim m_iUnrecoverableError As Int16 = 0

    ' stores if the last transfer was denied
    Dim m_boolDenied As Boolean = False

    ' stores if the last transfer was aborted
    Dim m_boolAborted As Boolean = False

    ' used to determine what to do on DisconnectEnd
    Dim m_boolShuttingDown As Boolean = False

    ' This method lists all devices found in addressbook
    Private Sub ListDevices()

        ' empty tree
        folderTree.Nodes.Clear()

        ' add all found in address book
        For Each deviceStored As StoredDevice In m_addressbook.Devices

            ' add nodes with device icon to treeview
            Dim deviceNode As TreeNode = New TreeNode(deviceStored.Name, 2, 2)
            deviceNode.Tag = deviceStored
            folderTree.Nodes.Add(deviceNode)
        Next
    End Sub

    Private Sub DisableUI()

        ' disable buttons
        disconnectBtn.Enabled = False
        putBtn.Enabled = False
        getBtn.Enabled = False
        delBtn.Enabled = False
        refreshBtn.Enabled = False
        createDirBtn.Enabled = False
        abortBtn.Enabled = False

        ' clear controls since we can't access the contents anymore
        folderTree.Nodes.Clear()
        fileListView.Items.Clear()
    End Sub

    Private Sub UpdateUI()
        fb.GetPathAsync()
    End Sub

    Private Sub fileListView_KeyPress(ByVal sender As System.Object, ByVal e As System.Windows.Forms.KeyPressEventArgs)
        ' enable download & delete button if any file is selected by keyboard
        getBtn.Enabled = fileListView.SelectedItems.Count > 0
        delBtn.Enabled = fileListView.SelectedItems.Count > 0
    End Sub

    Private Sub fileListView_DoubleClick(ByVal sender As System.Object, ByVal e As System.EventArgs)

        If fileListView.Items.Count > 0 Then

            Dim lvi As ListViewItem = fileListView.SelectedItems(0)

            Dim ff As FileFolder = DirectCast(lvi.Tag, FileFolder)

            If Not ff Is Nothing Then

                SaveFileDialog.FileName = ff.Name

                If SaveFileDialog.ShowDialog() = DialogResult.OK Then

                    Dim fs As FileStream = New FileStream(SaveFileDialog.FileName, FileMode.Create, FileAccess.Write)

                    fb.GetFileAsync(fs, ff.Name)
                End If
            End If
        End If
    End Sub

    Private Sub fileListView_DragDrop(ByVal sender As System.Object, ByVal e As System.Windows.Forms.DragEventArgs)

        If e.Data.GetDataPresent(DataFormats.FileDrop) Then

            Dim filenames As String() = DirectCast(e.Data.GetData(DataFormats.FileDrop), String())

            For Each filename As String In filenames

                Dim fs As FileStream = New FileStream(filename, FileMode.Open, FileAccess.Read)

                fb.PutFileAsync(fs, Path.GetFileName(filename))
            Next
        End If
    End Sub

    Private Sub fileListView_DragOver(ByVal sender As System.Object, ByVal e As System.Windows.Forms.DragEventArgs)
        e.Effect = DragDropEffects.Copy
    End Sub


    Private Sub fb_SetPathEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.SetPathEnd
        ' called when setpath is finished
        UpdateUI()
    End Sub

    Private Sub fb_CreateDirectoryEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.CreateDirectoryEnd
        ' called when new directory is created
        UpdateUI()
    End Sub

    Private Sub fb_DisconnectEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.DisconnectEnd

        If m_boolShuttingDown Then

            m_addressbook.Close()

            ' cancel any pending device discovery operations
            If Not m_network Is Nothing Then

                m_network.CancelDeviceDiscovery()
            End If

            If Not m_manager Is Nothing Then
                m_manager.Dispose()
            End If

            Close()
        Else
            ' disable user interface
            DisableUI()

            ' update status bar
            statusBarPanel.Text = "Disconnected."

            ' close stream if open
            Try

                If Not m_serviceCurrent Is Nothing And Not m_streamCurrent Is Nothing Then
                    m_streamCurrent.Close()
                End If

            Finally

                m_serviceCurrent = Nothing
                m_streamCurrent = Nothing
                m_deviceCurrent = Nothing
            End Try

            ' list the devices found in addressbook
            ListDevices()

            ' enable discover button so we can connect again
            discoverBtn.Enabled = True
        End If
    End Sub

    Private Sub fb_Error(ByVal sender As Object, ByVal eventArgs As Franson.Protocols.Obex.ObexEventArgs) Handles fb.Error

        Dim errorArgs As ObexErrorEventArgs = DirectCast(eventArgs, ObexErrorEventArgs)

        Select Case errorArgs.Error

            Case Franson.Protocols.Obex.ErrorCode.StreamError

                ' count one more unrecoverable error
                m_iUnrecoverableError = m_iUnrecoverableError + 1

                ' if there was an error we should clear this stream, it's not valid anymore
                If Not m_streamCurrent Is Nothing Then

                    m_streamCurrent.Close()

                End If

            Case Franson.Protocols.Obex.ErrorCode.Forbidden
                MessageBox.Show(errorArgs.Message)

            Case Franson.Protocols.Obex.ErrorCode.NoConnection

                m_iUnrecoverableError = m_iUnrecoverableError + 1
                MessageBox.Show("Could not establish a connection or the connection has been lost to the Obex device.")

            Case Franson.Protocols.Obex.ErrorCode.NotFound

                MessageBox.Show("File or folder not found.")

            Case Franson.Protocols.Obex.ErrorCode.SendDenied

                ' this transfer was denied!
                m_boolDenied = True

                ' this requires a disconnect, so we disconnect and connect again
            Case Franson.Protocols.Obex.ErrorCode.SecurityAbort

                m_boolAborted = True

                ' a security abort expects a disconnect too.
                ' so you could pass a disconnect here and keep track of it's a real disconnect
                ' we're just faking and connecting straight away to give the server what it wants
                ' this sample does not implement this kind of solution though

                ' if you want to do this really nice, you'll keep track of the path too, and restore it
                ' as well. this solution does put us back to root folder (the inbox)


            Case Franson.Protocols.Obex.ErrorCode.Aborted

                m_boolAborted = True

                MessageBox.Show(errorArgs.Message)

        End Select

        ' only need to inform user once
        If m_iUnrecoverableError = 1 Then

            MessageBox.Show(errorArgs.Message)

            ' disable UI
            DisableUI()

            ' restore devices in the tree
            ListDevices()

            m_serviceCurrent = Nothing
            m_streamCurrent = Nothing
        End If
    End Sub

    Private Sub fb_ConnectEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.ConnectEnd
        ' when connection is established


        ' update UI to display any files and folders in the current directory
        ' if there was a unrecoverable error there's no point in trying to talk to device though
        If Not m_deviceCurrent Is Nothing And m_iUnrecoverableError = 0 Then

            UpdateUI()
            statusBarPanel.Text = "Connected to " + DirectCast(m_deviceCurrent, Device).Name

            ' enable buttons needed
            disconnectBtn.Enabled = True
            putBtn.Enabled = True
            delBtn.Enabled = True
            refreshBtn.Enabled = True
            createDirBtn.Enabled = True

        Else

            statusBarPanel.Text = "Some error has occured."

            ListDevices()
        End If
    End Sub

    Private Sub fb_GetPathEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.GetPathEnd

        If m_iUnrecoverableError = 0 Then

            ' when get path finished - clear controls
            folderTree.Nodes.Clear()
            fileListView.Items.Clear()

            Dim i = 0
            ' step through entire cache
            For i = 0 To fb.Items.Count - 1

                ' get FileFolder object at current index position
                Dim ff As FileFolder = fb.Items(i)

                ' declare variables for nodes
                Dim node As TreeNode = Nothing
                Dim ListNode As ListViewItem = Nothing


                ' if FileFolder is a folder
                If ff.IsFolder Then

                    ' create a TreeView node
                    node = New TreeNode(ff.Name, 0, 0)

                    ' store the FileFolder object if we want to access some of its properties in the UI
                    node.Tag = ff

                    ' add the object to the treeview
                    folderTree.Nodes.Add(node)

                Else

                    ' if FileFolder is not a folder it is by definition a file...

                    ' create a ListView item
                    ListNode = New ListViewItem(ff.Name, 1)

                    ' add size and modification date to sub-columns (we could add other properties here too - like last access etc.)
                    ' if size is unknown it will return -1, and if modification date is unknown it will return DateTime.MinValue
                    ' filesize is always displayed in kb, so if there are less than 1024 bytes it will be treated as 1 kb, unless
                    ' it's 0, when it will display 0 kb
                    If ff.FileSize > 0 Then
                        If ff.FileSize > 1024 Then
                            ListNode.SubItems.Add(Convert.ToString(ff.FileSize \ 1024))
                        Else
                            ListNode.SubItems.Add("1 kb")
                        End If
                    Else
                        ListNode.SubItems.Add("0 kb")
                    End If

                    If ff.FileModified = DateTime.MinValue Then
                        ListNode.SubItems.Add("n/a")
                    Else
                        ListNode.SubItems.Add(ff.FileModified.ToString())
                    End If

                    ' store the FileFolder object if we want to access some of its properties in the UI
                    ListNode.Tag = ff

                    ' add the item to the listview instead
                    fileListView.Items.Add(ListNode)
                End If

            Next

            ' insert step back (..)
            folderTree.Nodes.Insert(0, New TreeNode("..", 0, 0))

            ' disable download & delete button
            getBtn.Enabled = False
            delBtn.Enabled = False

            ' enable the refresh button
            refreshBtn.Enabled = True
        End If
    End Sub

    Private Sub FindServiceAndConnect()

        If Not m_deviceCurrent Is Nothing Then

            Try

                ' Add a DiscoveryListener so we get service discovery events

                ' Cancel any on-going device discovery (not that there should be any going on)
                m_network.CancelDeviceDiscovery()

                ' only find OBEX FTP Services - this is done in UI-thread so let's do it asynchronously
                m_deviceCurrent.DiscoverServicesAsync(ServiceType.OBEXFileTransfer)

                statusBarPanel.Text = "Trying to connect..."

            Catch exc As Exception

                ' enable discover button if something happened
                discoverBtn.Enabled = True

                MessageBox.Show(exc.Message)
            End Try
        End If
    End Sub

    Private Sub m_network_DeviceDiscoveryCompleted(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_network.DeviceDiscoveryCompleted
        ' add all found devices to addressbook
        For Each device As Device In m_network.Devices

            ' add device to AddressBook
            m_addressbook.Add(device.Address, device.Name)

        Next

        discoverBtn.Enabled = True

        ' list all found and stored devices
        ListDevices()

        ' Search for devices on the network ended.
        statusBarPanel.Text = Convert.ToString(m_network.Devices.Length) + " device(s) in your vicinity."

        ' re-enable the folder tree
        folderTree.Enabled = True

    End Sub

    Private Sub m_network_DeviceDiscoveryStarted(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_network.DeviceDiscoveryStarted
        ' Search for devices on the network started

        statusBarPanel.Text = "Searching for bluetooth devices..."

        ' disable folderTree. You can't attempt to connect to anything now
        folderTree.Enabled = False

    End Sub

    Private Sub m_deviceCurrent_ServiceDiscoveryCompleted(ByVal sender As Object, ByVal eventArgs As Franson.BlueTools.BlueToolsEventArgs) Handles m_deviceCurrent.ServiceDiscoveryCompleted
        Try

            ' reset error status - there has been no errors on this session
            m_iUnrecoverableError = 0

            ' if we have no streams or services already connected
            If m_streamCurrent Is Nothing Then

                ' Get the remote device that raised the event
                m_deviceCurrent = DirectCast(sender, RemoteDevice)

                ' You find an array of all found services here - the list is complete since this is ServiceDiscoverCompleted
                Dim discovery As DiscoveryEventArgs = DirectCast(eventArgs, DiscoveryEventArgs)
                Dim services As Service() = DirectCast(discovery.Discovery, Service())


                ' if services.Length is above 0 it means we found an Obex FTP service
                If services.Length > 0 Then

                    ' store current service
                    m_serviceCurrent = DirectCast(services(0), RemoteService)

                    ' get the stream to the Obex FTP service
                    m_streamCurrent = m_serviceCurrent.Stream

                    If Not m_streamCurrent Is Nothing Then

                        ' attempt to connect asynchronously to Obex FTP server
                        fb.ConnectAsync(m_streamCurrent)

                    End If
                Else

                    ' enable discover button if we found no appropriate service
                    discoverBtn.Enabled = True

                    ' if this device did not support OBEX FTP display message in status bar
                    statusBarPanel.Text = "No service for Obex FTP available on device."

                    ' Update device list
                    ListDevices()
                End If
            End If

        Catch exc As Exception

            m_serviceCurrent = Nothing
            m_streamCurrent = Nothing
            m_deviceCurrent = Nothing

            ' display error message in window
            statusBarPanel.Text = exc.Message

            ' go back to showing devices
            ListDevices()
        End Try
    End Sub

    Private Sub fb_GetFileBegin(ByVal sender As Object, ByVal args As Franson.Protocols.Obex.ObexEventArgs) Handles fb.GetFileBegin

        ' this transfer hasn't been denied (yet!)
        m_boolDenied = False
        ' this transfer hasn't been aborted (yet!)
        m_boolAborted = False

        ProgressBar.Visible = True
        Dim copyArgs As ObexCopyEventArgs = DirectCast(args, ObexCopyEventArgs)

        ' must see if Size is -1, that means it is unknown
        If Not copyArgs.Size = 1 Then
            ProgressBar.Maximum = copyArgs.Size
        Else
            ProgressBar.Maximum = 0
        End If

        abortBtn.Enabled = True
        refreshBtn.Enabled = False
        createDirBtn.Enabled = False
        putBtn.Enabled = False
        getBtn.Enabled = False
        delBtn.Enabled = False
        ProgressBar.Value = 0

    End Sub

    Private Sub fb_GetFileEnd(ByVal sender As Object, ByVal args As Franson.Protocols.Obex.ObexEventArgs) Handles fb.GetFileEnd
        Dim copyArgs As ObexCopyEventArgs = DirectCast(args, ObexCopyEventArgs)

        ' check if Stream is available before closing
        If Not copyArgs.Stream Is Nothing Then

            copyArgs.Stream.Close()


            ProgressBar.Visible = False

            ' ...if sending was denied
            If m_boolDenied Then

                statusBarPanel.Text = "File transfer denied by device."

                ' ..if sending was aborted
            ElseIf m_boolAborted Then

                statusBarPanel.Text = "Copy was aborted."

                ' ...else if there has been no unrecoverable errors
            ElseIf m_iUnrecoverableError = 0 Then

                statusBarPanel.Text = "Copy complete."

                ' .. or if there has been unrecoverable errors
            Else

                statusBarPanel.Text = "There was an error with the copy and the connection was lost."

            End If

            ' Upload button should always be re-enabled (Download should only be re-enabled when a file is selected)
            putBtn.Enabled = True
            delBtn.Enabled = True

            ' abort button should only be available while copying
            abortBtn.Enabled = False

            ' if this was a putfile operation, there is a new file on the remote device
            ' if there was an unrecoverable error we can't access the device though...
            If ((copyArgs.Event = Franson.Protocols.Obex.EventType.PutFileEnd) And (m_iUnrecoverableError = 0)) Then

                refreshBtn.Enabled = True
                createDirBtn.Enabled = True
                delBtn.Enabled = False

                UpdateUI()
            End If
        End If
    End Sub

    Private Sub fb_PutFileBegin(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.PutFileBegin

        ' call same event handler as GetFile
        fb_GetFileBegin(sender, e)

    End Sub

    Private Sub fb_PutFileProgress(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.PutFileProgress

        ' call same event handler as GetFile

        fb_GetFileProgress(sender, e)

    End Sub

    Private Sub fb_PutFileEnd(ByVal sender As Object, ByVal e As Franson.Protocols.Obex.ObexEventArgs) Handles fb.PutFileEnd

        ' call same event handler as GetFile

        fb_GetFileEnd(sender, e)

    End Sub

    Private Sub fb_GetFileProgress(ByVal sender As Object, ByVal args As Franson.Protocols.Obex.ObexEventArgs) Handles fb.GetFileProgress

        Dim copyArgs As ObexCopyEventArgs = DirectCast(args, ObexCopyEventArgs)

        ' if size is unknown, ObexCopyEventArgs.Size will return -1
        If Not copyArgs.Size = -1 Then

            If Not copyArgs.Size = ProgressBar.Maximum Then

                ProgressBar.Maximum = copyArgs.Size


                ProgressBar.Value = copyArgs.Position

                statusBarPanel.Text = Convert.ToString(copyArgs.Position) + "/" + Convert.ToString(copyArgs.Size)
            End If
        Else

            ' show user that we are copying but we don't know the full size
            statusBarPanel.Text = Convert.ToString(copyArgs.Position) + "/ ?"
        End If
    End Sub

    Private Sub toolBar_ButtonClick(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolBarButtonClickEventArgs) Handles toolBar.ButtonClick
        ' check which button was clicked and act appropriately...

        ' discover button
        If e.Button Is discoverBtn Then

            ' empty the addressbook so all names and such are updated
            m_addressbook.Clear()

            ' disable discover button
            discoverBtn.Enabled = False

            ' scan for devices
            m_network.DiscoverDevicesAsync()

            ' delete button
        ElseIf e.Button Is delBtn Then

            ' if a foldertree is selected
            If Not folderTree.SelectedNode Is Nothing And Not folderTree.SelectedNode.Tag Is Nothing Then

                Dim ff As FileFolder = DirectCast(folderTree.SelectedNode.Tag, FileFolder)

                fb.DeleteAsync(ff.Name)

                UpdateUI()

            ElseIf fileListView.SelectedItems.Count > 0 And Not fileListView.SelectedItems(0).Tag Is Nothing Then

                Dim ff As FileFolder = DirectCast(fileListView.SelectedItems(0).Tag, FileFolder)

                fb.DeleteAsync(ff.Name)

                UpdateUI()

            Else
                MessageBox.Show("No file or folder selected.")
            End If

        ElseIf e.Button Is putBtn Then

            If OpenFileDialog.ShowDialog() = DialogResult.OK Then

                Try
                    For Each filename As String In OpenFileDialog.FileNames

                        ' open a stream to the file so we can read the data from it
                        Dim fs As FileStream = New FileStream(filename, FileMode.Open, FileAccess.Read)

                        ' store the file! (only the filename, not the path)
                        fb.PutFileAsync(fs, Path.GetFileName(filename))
                    Next

                Catch exc As Exception

                    ' show some error if we failed to open the FileStream
                    MessageBox.Show(exc.Message)
                End Try
            End If

        ElseIf e.Button Is getBtn Then

            ' if user wants to download files

            ' check that there are selected files to download
            If fileListView.SelectedItems.Count > 0 Then

                ' iterate all files
                For Each lvItem As ListViewItem In fileListView.SelectedItems

                    ' check that we have a FileFolder with info
                    If Not lvItem.Tag Is Nothing Then

                        ' get the FileFolder object
                        Dim ff As FileFolder = DirectCast(lvItem.Tag, FileFolder)

                        ' propose the same name
                        SaveFileDialog.FileName = ff.Name

                        ' ask where to store the file
                        If SaveFileDialog.ShowDialog() = DialogResult.OK Then

                            ' open a file stream to where the local store should be kept
                            Dim fs As FileStream = New FileStream(SaveFileDialog.FileName, FileMode.Create)

                            ' ask Obex to download it!
                            fb.GetFileAsync(fs, ff.Name)
                        End If
                    End If
                Next
            End If
        ElseIf e.Button Is abortBtn Then

            fb.Abort()

        ElseIf e.Button Is refreshBtn Then

            UpdateUI()

            refreshBtn.Enabled = False

        ElseIf e.Button Is disconnectBtn Then

            fb.DisconnectAsync()

        ElseIf e.Button Is createDirBtn Then

            Dim input As InputForm = New InputForm
            If input.ShowDialog(Me) = DialogResult.OK Then

                fb.CreateDirectoryAsync(input.FolderName)

            End If
        End If
    End Sub

    Private Sub folderTree_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles folderTree.DoubleClick
        Dim node As TreeNode = folderTree.SelectedNode

        If fb.Connected And m_iUnrecoverableError = 0 Then

            ' this checks if there is a folder selected..
            If Not node Is Nothing Then

                ' check so it is a node that we have ourselves added (with a FileFolder attached) that is being double-clicked
                If Not node.Tag Is Nothing Then

                    Dim ff As FileFolder = DirectCast(node.Tag, FileFolder)

                    fb.SetPathAsync(ff.Name)

                Else

                    ' All nods that do not have a FileFolder attached is treated as step-back
                    fb.SetPathAsync("..")
                End If
            End If
        Else
            If Not node Is Nothing Then

                ' disable discover button
                discoverBtn.Enabled = False

                ' get stored device
                Dim deviceStored As StoredDevice = DirectCast(folderTree.SelectedNode.Tag, StoredDevice)

                ' empty treeview
                folderTree.Nodes.Clear()

                ' set new device
                m_deviceCurrent = DirectCast(m_network.ConnectDevice(deviceStored.Address, deviceStored.Name), RemoteDevice)

                ' try to connec to Obex FTP
                FindServiceAndConnect()
            End If
        End If
    End Sub

    Private Sub folderTree_AfterSelect(ByVal sender As Object, ByVal e As System.Windows.Forms.TreeViewEventArgs) Handles folderTree.AfterSelect

        If fb.Connected Then
            delBtn.Enabled = (Not folderTree.SelectedNode Is Nothing) And (Not folderTree.SelectedNode.Tag Is Nothing)
        End If

    End Sub

    Private Sub fileListView_ItemActivate(ByVal sender As Object, ByVal e As System.EventArgs) Handles fileListView.ItemActivate

        ' activate buttons when items are selected
        delBtn.Enabled = fileListView.SelectedItems.Count > 0
        getBtn.Enabled = fileListView.SelectedItems.Count > 0

    End Sub

    Private Sub fileListView_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles fileListView.MouseUp

        ' enable download & delete button if any file is selected by mouse
        getBtn.Enabled = fileListView.SelectedItems.Count > 0
        delBtn.Enabled = fileListView.SelectedItems.Count > 0

    End Sub

    Private Sub MainForm_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing

        ' if this boolean is set we have already done this
        If m_boolShuttingDown Then
            Return
        End If

        If Not fb Is Nothing And fb.Connected Then

            fb.DisconnectAsync()

            e.Cancel = True

            ' set this boolean so we do the correct things on DisconnectEnd
            m_boolShuttingDown = True

            ' don't continue to execute this method
            Return
        End If

        If Not m_addressbook Is Nothing Then

            m_addressbook.Close()

        End If


        ' cancel any pending device discovery operations
        If Not m_network Is Nothing Then

            m_network.CancelDeviceDiscovery()
        End If

        Dispose()
    End Sub
End Class
