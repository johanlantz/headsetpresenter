Option Explicit On 

Imports Franson.BlueTools

Public Class Form1
    Inherits System.Windows.Forms.Form
    Friend WithEvents listDevices As System.Windows.Forms.ListBox
    Friend WithEvents listServices As System.Windows.Forms.ListBox
    Friend WithEvents bDiscover As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents txtData As System.Windows.Forms.TextBox
    Friend WithEvents labelStatus As System.Windows.Forms.Label

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        MyBase.Dispose(disposing)
    End Sub

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents bClose As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.listDevices = New System.Windows.Forms.ListBox
        Me.listServices = New System.Windows.Forms.ListBox
        Me.bDiscover = New System.Windows.Forms.Button
        Me.Label1 = New System.Windows.Forms.Label
        Me.Label2 = New System.Windows.Forms.Label
        Me.txtData = New System.Windows.Forms.TextBox
        Me.labelStatus = New System.Windows.Forms.Label
        Me.bClose = New System.Windows.Forms.Button
        '
        'listDevices
        '
        Me.listDevices.Location = New System.Drawing.Point(16, 24)
        Me.listDevices.Size = New System.Drawing.Size(96, 132)
        '
        'listServices
        '
        Me.listServices.Location = New System.Drawing.Point(128, 24)
        Me.listServices.Size = New System.Drawing.Size(96, 132)
        '
        'bDiscover
        '
        Me.bDiscover.Location = New System.Drawing.Point(16, 168)
        Me.bDiscover.Size = New System.Drawing.Size(96, 24)
        Me.bDiscover.Text = "Discover"
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(16, 8)
        Me.Label1.Size = New System.Drawing.Size(88, 16)
        Me.Label1.Text = "Devices"
        '
        'Label2
        '
        Me.Label2.Location = New System.Drawing.Point(128, 8)
        Me.Label2.Size = New System.Drawing.Size(88, 16)
        Me.Label2.Text = "Services"
        '
        'txtData
        '
        Me.txtData.Location = New System.Drawing.Point(16, 208)
        Me.txtData.Size = New System.Drawing.Size(208, 20)
        Me.txtData.Text = ""
        '
        'labelStatus
        '
        Me.labelStatus.Location = New System.Drawing.Point(16, 240)
        Me.labelStatus.Size = New System.Drawing.Size(192, 16)
        Me.labelStatus.Text = "OK"
        '
        'bClose
        '
        Me.bClose.Location = New System.Drawing.Point(152, 168)
        Me.bClose.Size = New System.Drawing.Size(72, 24)
        Me.bClose.Text = "Close"
        '
        'Form1
        '
        Me.Controls.Add(Me.bClose)
        Me.Controls.Add(Me.labelStatus)
        Me.Controls.Add(Me.txtData)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.bDiscover)
        Me.Controls.Add(Me.listServices)
        Me.Controls.Add(Me.listDevices)
        Me.MinimizeBox = False
        Me.Text = "FindAndConnect VB.NET"

    End Sub

#End Region

    Dim m_manager As Manager

    Dim m_stream As ServiceStream
    Dim m_buffer(25) As Byte

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim license As New License
        license.LicenseKey = "HU5UZerz41POJX58liigUkbRWvw0QRKbHY5X"

        Try
            m_manager = Manager.GetManager()

            ' Call any events in GUI thread!
            m_manager.Parent = Me

            ' Events called in a new thread "multi-threading"
            ' m_manager.Parent = Nothing

        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try

        labelStatus.Text = "Click on Discover!"
        bClose.Enabled = False
    End Sub

    Private Sub bDiscover_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bDiscover.Click

        bDiscover.Enabled = False
        labelStatus.Text = "Discovering devices..."

        ' Clear lists
        listDevices.Items.Clear()
        listServices.Items.Clear()
        txtData.Text = ""

        ' Redraw form
        Refresh()

        Dim networks() As Network
        Dim nwork As Network

        Try
            networks = m_manager.Networks

            ' On Pocket PC there is only one network
            nwork = networks(0)

        Catch ex As Exception
            MessageBox.Show("BlueTooth network not found")
        End Try

        ' Get list of devices
        Dim devices() As Device

        ' NOTE! Use Network.DiscoverDevicesAsync() for event driven lookup (async)
        devices = nwork.DiscoverDevices()

        Dim inx As Integer
        For inx = 0 To devices.Length - 1
            listDevices.Items.Add(devices(inx))
        Next

        bDiscover.Enabled = True
        labelStatus.Text = "Click on a device!"

    End Sub

    Private Sub listDevices_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles listDevices.SelectedIndexChanged

        Dim remote_device As RemoteDevice
        remote_device = listDevices.SelectedItem

        labelStatus.Text = "Services for " + remote_device.Name + "..."
        listServices.Items.Clear()
        txtData.Text = ""

        Refresh()

        Dim services() As Service

        ' NOTE! Use RemoteDevice.DiscoverServicesAsync for event driven lookup (async)

        ' Use ServiceType.RFCOMM to look up all services on one device
        ' Use e.g. ServiceType.SerialPort to list only serial port services
        services = remote_device.DiscoverServices(Franson.BlueTools.ServiceType.RFCOMM)

        Dim inx As Integer
        For inx = 0 To services.Length - 1
            listServices.Items.Add(services(inx))
        Next

        labelStatus.Text = "Click on a service!"

    End Sub

    Private Sub listServices_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles listServices.SelectedIndexChanged
        Dim remote_service As RemoteService
        remote_service = listServices.SelectedItem

        labelStatus.Text = "Connecting to " + remote_service.Name + "..."
        Refresh()

        Try
            ' Open connection to service
            m_stream = remote_service.Stream

            ' Start async read
            m_stream.BeginRead(m_buffer, 0, m_buffer.Length, New AsyncCallback(AddressOf ReadCallback), m_stream)

        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try

        labelStatus.Text = "Receiving data"
        bClose.Enabled = True
        bDiscover.Enabled = False

    End Sub

    Private Sub ReadCallback(ByVal result As IAsyncResult)
        Dim buffer() As Byte
        Dim len As Integer

        ' result is always a BlueToolsAsyncResult object
        Dim blueAsyncResult As BlueToolsAsyncResult = result

        ' We passed on buffer as custom object, you can pass on any object here. We passed the stream object
        Dim stream As ServiceStream = blueAsyncResult.AsyncState

        ' The buffer used for reading can be found in the result object.
        buffer = blueAsyncResult.Buffer

        Try
            ' EndRead() must always be called!
            ' If stream has been closed due to an error, we'll have an excpetion here
            len = m_stream.EndRead(result)

            Dim str As String
            Dim enc As New System.Text.ASCIIEncoding
            str = enc.GetString(buffer, 0, len)

            txtData.Text = str

            ' Start new async read
            m_stream.BeginRead(m_buffer, 0, m_buffer.Length, New AsyncCallback(AddressOf ReadCallback), m_stream)

        Catch ex As ObjectDisposedException
            ' Thrown if stream has been closed.
            bClose.Enabled = False
            bDiscover.Enabled = True
            listServices.Enabled = True

            labelStatus.Text = ex.Message
        End Try

    End Sub

    Private Sub Form1_Closing(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles MyBase.Closing
        If Not m_manager Is Nothing Then
            m_manager.Dispose()
        End If
    End Sub

    Private Sub bClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles bClose.Click
        If Not m_stream Is Nothing Then
            m_stream.Close()
        End If

        bClose.Enabled = False
        bDiscover.Enabled = True

        labelStatus.Text = "Stop!"

    End Sub

End Class
