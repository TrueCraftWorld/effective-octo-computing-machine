#include "mocknode.h"

MockNode::MockNode(quint16 port) : m_tcpServer(port)
{
	connect(this, &MockNode::signalNeighbourListUpdated, &m_tcpServer, &Server::slotConnectSocket);
	connect(&m_tcpServer, &Server::signalSocketDisconnected, this, &MockNode::slotTcpSocketDisonnected);
	connect(&m_tcpServer, &Server::signalSocketConnected, this, &MockNode::slotTcpSocketConnected);

	m_timerCheckerExistedTcpConnections = new QTimer(this);
	m_timerCheckerExistedTcpConnections->setInterval(TIME_CHECK_CONNECTION_MSEC);
	m_timerCheckerExistedTcpConnections->start();

	connect(m_timerCheckerExistedTcpConnections, &QTimer::timeout, this, &MockNode::slotCheckConnections);
}

void MockNode::SetNewConnection(const NodeData& data)
{
	m_nodeInfo.neighbour_nodes.append(data);
	emit signalNeighbourListUpdated(data.ip, data.port);
}

void MockNode::slotCheckConnections()
{
	for (const auto& info : m_nodeInfo.neighbour_nodes)
	{
		m_tcpServer.SendMessageToNode(info.socket, "");
	}
}

void MockNode::slotTcpSocketDisonnected(QTcpSocket* socket)
{
	m_nodeInfo.neighbour_nodes.erase(
		std::remove_if(
			m_nodeInfo.neighbour_nodes.begin(),
			m_nodeInfo.neighbour_nodes.end(),
			[socket](const NodeData& data) { return data.ip == socket->peerAddress() && data.port == socket->peerPort(); }),
		m_nodeInfo.neighbour_nodes.end());
	socket->deleteLater();
}

void MockNode::slotTcpSocketConnected(QTcpSocket* socket, QHostAddress ip4, quint16 port)
{
	for (auto& info : m_nodeInfo.neighbour_nodes)
	{
		if (info.ip == ip4 && info.port == port)
		{
			info.socket = socket;
			break;
		}
	}
}