
list_Q=dir('Q*.csv');

while 1
  for i=1:length(list_Q)
    Q_i=csvread(list_Q(i).name);
    surf(Q_i);
    title(list_Q(i).name); 
    pause();
  end
%  pause(5);
end
