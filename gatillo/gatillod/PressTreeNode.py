
class PressTreeNode:

	def __init__( self, action_function, children ):
		self.action = action_function
		self.children = children


	def lookup( press_stack, node ):

		if press_stack == []:
			node.action_function()

		elif node[press_stack.head] == None:
			self.lookup( press_stack.tail, node )

		else:
			self.lookup( press_stack.tail, node[press_stack.head] )


