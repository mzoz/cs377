
public class BankAccount {
	
	private int balance;
	
	public BankAccount(){
		this.balance = 0;
	}
	
	public synchronized void increaseAmount(int amount){
		
		this.balance += amount;
		notify();
		
	}
	
	public synchronized void decreaseAmount(int amount) throws InterruptedException{
		
		while(this.balance < amount){
			wait();
		}
		this.balance -= amount;
		
	}
	
	public static void main(String[] args){
		System.out.println("start running...");
		
		BankAccount myAccount = new BankAccount();
		
		Runnable d = new Deposit(myAccount);
		Runnable w = new Withdraw(myAccount);
		
		Thread t1 = new Thread(d);
		Thread t2 = new Thread(w);
		
		t1.start();
		t2.start();
		
	}

}
