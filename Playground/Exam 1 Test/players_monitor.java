class players_monitor { 
	private volatile int move = 1;
	public synchronized void MoveRed(){ 
		while (move != 1){ 
			wait(); 
		} 
		print("Red Piece Moved"); 
		move = 2; 
		notifyAll(); 
	} 

	public synchronized void MoveBlue(){ 
		while(move != 2){ 
			wait(); 
		} 
		print("Blue Piece Moved"); 
		move = 3; 
		notifyAll(); 
	} 
	public synchronized void MoveGreen(){ 
		while(move != 3){ 
			wait(); 
		} 
		print("Green Piece Moved"); 
		move = 1; 
		notifyAll(); 
	} 
} 
