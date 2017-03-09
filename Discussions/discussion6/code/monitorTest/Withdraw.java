
public class Withdraw implements Runnable{
	
	private BankAccount account;
	
	public Withdraw(BankAccount account){
		this.account = account;
	}
	
	public void run(){
		
		System.out.println("start withdraw 250.");
		try{
			
			this.account.decreaseAmount(250);
			
		}catch(InterruptedException e){
			System.out.println("Exception withdraw.");
		}
		System.out.println("finish withdraw 250.");
		
	}
}
