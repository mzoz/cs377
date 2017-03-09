
public class Deposit implements Runnable{
	
	private BankAccount account;
	
	public Deposit(BankAccount account){
		this.account = account;
	}
	
	public void run(){
		System.out.println("start deposit 100.");
		this.account.increaseAmount(100);
		System.out.println("finish deposit 100.");
		
		int millisec = 1000;
		try{
			Thread.sleep(millisec);
		}catch(InterruptedException e){
			System.out.println("Exception in sleep.");
		}
		
		System.out.println("start deposit 200.");
		this.account.increaseAmount(200);
		System.out.println("finish deposit 200.");
		
	}

}
