Imports System.Collections
Imports System.Xml
Imports System.IO
Imports Franson.BlueTools

' ObexDevice stores the address to a bluetooth supported device.
Public Class StoredDevice

    Dim m_address As Address = Nothing
    Dim m_name As String = ""

    ' Constructs new ObexDevice objects.
    Public Sub New(ByVal address As Address, ByVal name As String)

        ' store an Address to the address stored in the xml file.
        m_address = address

        ' store name
        m_name = name
    End Sub

    ' Returns the name of the device stored by this StoredDevice instance.
    Public ReadOnly Property Name() As String

        Get

            Return m_name
        End Get
    End Property

    ' Returns the BlueTools Address stored by this StoredDevice instance.
    Public ReadOnly Property Address() As Franson.BlueTools.Address

        Get
            Return m_address
        End Get
    End Property
End Class

Public Class AddressBook
    Dim m_hashAddressList As Hashtable = Nothing

    Public AddressBook()

    'constructor

    Public Sub New()
        m_hashAddressList = New Hashtable
    End Sub

    Public ReadOnly Property Devices() As StoredDevice()
        Get

            Return DirectCast(New ArrayList(m_hashAddressList.Values()).ToArray(GetType(StoredDevice)), StoredDevice())

        End Get
    End Property
    ' Load existing addresses from xml-file.
    Public Sub Load()

        Dim readerXML As XmlTextReader = New XmlTextReader("addressbook.xml")

        Try

            ' check if AddressBook xml-file exists
            If (File.Exists("addressbook.xml")) Then

                Dim xmlDoc As XmlDocument = New XmlDocument

                Try

                    xmlDoc.Load(readerXML)

                Catch exc As Exception

                    Console.WriteLine(exc.Message)
                End Try

                Dim xmlDeviceList As XmlNodeList = xmlDoc.GetElementsByTagName("Devices")

                If xmlDeviceList.Count > 0 Then

                    Dim xmlRootNode As XmlNode = xmlDeviceList.Item(0)

                    For Each node As XmlNode In xmlRootNode.ChildNodes

                        If Not (node.Attributes("address") Is Nothing) And (Not node.Attributes("name") Is Nothing) Then

                            m_hashAddressList.Add(node.Attributes("address").Value, New StoredDevice(New Address(node.Attributes("address").Value), node.Attributes("name").Value))

                        End If
                    Next

                End If
            End If
        Catch

            ' all exceptions are treated as the file is corrupt.
            ' thus we clean the address list.
            m_hashAddressList.Clear()
        End Try

        readerXML.Close()
    End Sub

    ' Empties the address book.
    Public Sub Clear()

        m_hashAddressList.Clear()
    End Sub

    ' Adds an Address object to the hash. If the object already exists, this will not create a duplicate.
    Public Sub Add(ByVal Address As Address, ByVal name As String)

        ' store address with string of address as key in hash
        ' if the address already exists in the hash, the new address it not added
        ' this is irrelevant though since they refer to the same device (Bluetooth addresses are unique!)

        If Not m_hashAddressList.ContainsKey(Address.ToString()) Then

            m_hashAddressList.Add(Address.ToString(), New StoredDevice(Address, name))
        End If

    End Sub

    ' Returns an Address object for the address given.
    Private Function GetAddress(ByVal address As String) As Address

        If (m_hashAddressList.ContainsKey(address)) Then
            Return DirectCast(m_hashAddressList(address), Address)
        Else
            Return Nothing
        End If

    End Function

    ' Close is used when closing the AddressBook and storing the data to a xml-file.
    Public Sub Close()
        Dim writer As XmlTextWriter = New XmlTextWriter("addressbook.xml", Nothing)

        writer.WriteStartDocument()

        writer.WriteStartElement("Devices")

        For Each device As StoredDevice In Devices

            writer.WriteStartElement("Device")
            writer.WriteAttributeString("name", device.Name)
            writer.WriteAttributeString("address", device.Address.ToString())
            writer.WriteEndElement()

        Next

        writer.WriteEndElement()
        writer.WriteEndDocument()
        writer.Close()
    End Sub

End Class
