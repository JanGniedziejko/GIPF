# GIPF
Board game simulation with a hexagon board

### Co to jest GIPF i na czym polega?
GIPF to gra deterministyczna, bez elementów losowych oraz ukrytej informacji dla dwóch graczy o sumie zerowej bez remisów. Zasady wersji oficjalnej dostępne są tutaj Zadanie polega na implementacji silnika tej gry dla jej uogólnionej wersji. Uogólnienie polega na tym, że można zmieniać właściwości gry opisane czteroma paramtrami (S, K, GW, GB) będącymi elementami wspomnianej wcześniej listy L parametrów opisujących grę:<br/>
**S -** rozmiar planszy wyrażony liczbą pól planszy wchodzących w skład każdego boku sześciobocznej planszy;<br/>
**K -** liczba pionów gracza która wyzwala zbieranie pionów, ta wartość nigdy nie powinna być mniejsza od dwóch ponieważ dla jedynki pozycja początkowa na planszy była by zabroniona (wszystkie piony należało by od razu usunąć). 

Wartość ta nie powinna być również większa od 2*S-1 ponieważ z tylu pól składa się "przekątna" planszy więc nie jest możliwe utworzenie dłuższego ciągu. Idąc dalej tym tropem można zauważyć, że wartość ta powinna być jeszcze mniejsza ponieważ w przypadku kiedy jest równa dokładnie 2*S-1 w grze nie można będzie zbijać pionków przeciwnika. Prowadzi to albo do niekończącej się rozgrywki polegającej na uwalnianiu w nieskończoność swoich pionów do rezerwy albo na zakończeniu rozgrywki poprzez zapchanie planszy, kiedy to żaden z graczy nie może dołożyć już żadnego piona do gry. Opisuje to jednoznacznie zasada z instrukcji: "Nie można wypchnąć piona poza obszar gry, czyli na kropkę po drugiej stronie linii.";
GW - liczba pionów należących do gracza białego, musi być większa niż trzy które są na starcie umieszczane na planszy gracza. jednak w przypadku kiedy będzie ich mało rozgrywka może się szybko zakończyć z powodu wyczerpania zasobów gracza, wtedy gracz rozpoczynający, czyli biały będzie zawsze na przegranej pozycji (dotyczy to przypadku równej liczby pionów u obydwu graczy);
GB - liczba pionów należących do gracza czarnego, może się różnić od GW.

Jak w podstawowej wersji każdy gracz zaczyna grę z 3 pionami na planszy, zawsze rozpoczyna biały. Piony umieszcza się na kropkach w rogach, a następnie przesuwa na pierwszy punkt w kierunku środka obszaru gry. Celem gracza jest, zdobycie pionów przeciwnika tak aby nie miał on żadnych pionów w rezerwie. Jednak jak zauważyliśmy wcześniej przy pewnych niefortunnych wariantach gry (niefortunnie dobrane parametry listy L) rozgrywka może się nie skończyć albo plansza może zostać zapchana. W pierwszym przypadku wynik rozwiązania gry będzie remisem, a w drugim gracz który nie może wykonać posunięcia będzie przegrywającym. jednak takie warianty gry będziemy traktować jako niewłaściwe starając się nie wykorzystywać ich w testach. Jak łatwo zauważyć, oryginalna gra opisana jest listą L=(4, 4, 15, 15) gdzie jedna z ostatnich dwóch wartości może być zwiększona maksymalnie o 3 w celu wprowadzenia asymetrii i tym samym wyrównaniu różnic w poziome gry obydwu graczy. Najmniejsza plansza jaką można sobie wyobrazić to plansza rozmiaru R = 2. Mniejsza plansza nie ma sensu ponieważ dla R = 1 obszar gry składał by się z tylko jednego pola gry, a musi ich być przynajmniej sześć aby pomieścić wszystkie początkowe (2x3) piony graczy.


**Silnik gry powinien pozwalać na:**
Wczytanie stanu gry, będzie on podawany w przypadkach testowych w następującym formacie. W pierwszej linii będą podane cztery liczby opisujące grę (zawartość listy L). W kolejnej linii pojawi się liczba pionów w rezerwach odpowiednio gracza grającego białymi i czarnymi oraz cyfra określająca aktywnego gracza. W kolejnych liniach znajdą się informacje opisujące stan planszy.

<img width="580" alt="Screenshot 2024-09-25 at 13 35 38" src="https://github.com/user-attachments/assets/dda837fa-03c6-43ff-b410-99ff6feddc89">

Zastosowanie posunięcia dla aktualnej planszy opisanego wg schematu: xN - yM gdzie xN to pole na które zostaje położony pion z rezerwy gracza, a pole yM to pole w kierunku którego ten właśnie położony pion zostaje przesunięty. Notacja ta jest zapożyczona z programu Gipf for One (GF1) i wygląda jak na poniższym rysunku: 

<img width="619" alt="Screenshot 2024-09-25 at 13 36 31" src="https://github.com/user-attachments/assets/84f8d32e-6e64-449a-b866-3c337228d264">

Zauważmy, że sąsiednie kropki nie będą stosowane w reprezentacji planszy, ponieważ używane są one jedynie podczas pierwszej fazy posunięcia, kiedy gracz kładzie piona z rezerw. Po położeniu piona na kropce gracz musi go przesunąć, jeśli takie przesunięcie spowoduje wypchnięcie piona po przeciwnej stronie na kropkę, to jest ono niedozwolone. W takim przypadku silnik gry, powinien zapamiętać to posunięcie jako błędne i nie powinien już analizować kolejnych posunięć. W przypadku prośby o wydrukowanie stanu planszy będzie drukowany jej stan bezpośrednio poprzedzający ten niedozwolony ruch. W przypadku gdy w tym samym czasie powstanie więcej rzędów składających się z 4 (bądź więcej) pionów tego samego koloru i rzędy te przecinają się, rozkaz musi zawierać informację który rząd ma być usunięty, w innym przypadku będzie to błędne posunięcie i konsekwencje będą identyczne jak w opisanych wcześniej przypadkach.

1. Wydrukowanie stanu planszy w postaci identycznej jak przy jej wczytywaniu, tak aby możliwa była ewentualna rozgrywka pomiędzy dwoma programami.
   
2. Wydrukowanie stanu gry:
- in_progress - gra się nie skończyła
- white_win - wygrał rozpoczynający
- black_win - wygrał gracz drugi
- dead_lock <kolor> - gracz którego tura właśnie trwa a on nie może wykonać posunięcia kolor gracza, dodatkowo drukujemy kolor gracza
- bad_move <kolor> <xN - yM> - gracz wykonał błędne posuniecie, dodatkowo drukujemy kolor gracza i komendę która wygenerowała błędne posunięcie

3. Generowanie wszystkich możliwych posunięć aktywnego gracza.

4. Ocenę gry w postaci odpowiedzi na pytanie - czy gra się zakończyła i czy gracz wygrał/zremisował/przegrał? Zauważmy, że jest to równoważne z odpowiedzią na pytanie - czy aktywny gracz może wykonać jakiekolwiek posunięcie. jeśli nie to przegrał a jego przeciwnik wygrał. Remis może nastąpić tylko w przypadku stwierdzenia, że gra nigdy się nie skończy, jednak na potrzeby tego zadania będziemy starali się nie doprowadzać do takich sytuacji.
  
5. W zoptymalizowanej wersji, w przypadku kiedy przynajmniej jedno z posunięć aktywnego gracza prowadzi do jego wygranej, silnik powinien wygenerować tylko jedno z tych posunięć.

### Wymagania implementacyjne.

**Program powinien reagować na następujące komendy:**
**LOAD_GAME_BOARD** po której podawane są parametry gry oraz stan planszy. Przykładowo, wczytanie oryginalnej planszy będzie wyglądało następująco:
<img width="130" alt="Screenshot 2024-09-25 at 13 40 32" src="https://github.com/user-attachments/assets/b7422f53-00c3-4829-b23e-91df75b19775">
Należy sprawdzić czy podany stan planszy jest poprawny, czy suma pionów obydwu graczy na planszy i w rezerwach jest poprawna, czyli identyczna z podaną w liście L. Należy sprawdzić czy nie zapomnieliśmy usunąć ciągów pionów, które spełnią kryterium wyzwalające ich zbieranie.

**PRINT_GAME_BOARD** drukuje aktualny stan planszy, po wczytaniu gry powinien to być dokładnie ten sam wydruk który został przekazany podczas wczytywania. Oczywiście zakładając, że był on poprawny.
**DO_MOVE <xN - yM>** wykonuje przekazany ruch dla aktywnego gracza na aktualnej planszy. Notacja posunięć powinna być zgodna z tą z programu Gipf for One (GF1) W przypadku gdy będzie on poprawny otrzymamy nową planszę a aktywny gracz zmieni się na przeciwnika. Może okazać się, że gra się zakończy jeśli przeciwnik nie będzie miał już pionów w rezerwie. W przypadku gdy ruch będzie niepoprawny należy ustalić odpowiedni stan gry, czyli "bad_move" oraz zapamiętać gracza i ruch które ten stan wywołały. W przypadku kiedy gracz musi zdjąć piony z planszy a istnieje kilka takich możliwości, musi on zadecydować które piony chce zdjąć. Ta informacja musi zostać dodana do komendy w postaci współrzędnych tych pionów. Współrzędne pionów są podawane wg schematu opisanego przykładem początkowej planszy dla standardowej wesji gry:
<img width="224" alt="Screenshot 2024-09-25 at 13 41 18" src="https://github.com/user-attachments/assets/5421e82e-9ca6-4cff-a988-69a8334b4a66">

Zatem w przypadku ruchu który doprowadzi do niejednoznacznej sytuacji wymagającej dookreślenie które piony trzeba zdjąć. Po "normalnym" rozkazie podawany jest kolor gracza którego dotyczy wybór a nastepnie ciąg współrzędnych określających zdejmowane piony. Rozkaz będzie wyglądał nastepująco:
DO_MOVE <xN - yM> [w:|b:] x1 xn

W przypadku kiedy tych wyborów będzie więcej zostana one podane w postaci kolejnych ciągów indeksów.<br/>
**PRINT_GAME_STATE** drukuje aktualny stan gry ("in_progress", "bad_move", itp.).<br/>
**GEN_ALL_POS_MOV** drukuje listę wszystkich posunięć (bez powtórzeń) prowadzących do unikalnych stanów planszy.<br/>
**GEN_ALL_POS_MOV_EXT** działa identycznie jak GEN_ALL_POS_MOV z wyjątkiem przypadku kiedy istnieje dowolne posunięcie wygrywające, wtedy drukuje tylko jedno z tych posunięć.<br/>
**GEN_ALL_POS_MOV_NUM** drukuje liczbę wszystkich posunięć jakie otrzymujemy w wyniku działania komendy **GEN_ALL_POS_MOV**.<br/>
**GEN_ALL_POS_MOV_EXT_NUM** działa identycznie jak GEN_ALL_POS_MOV_NUM z wyjątkiem przypadku kiedy istnieje dowolne posunięcie wygrywające, wtedy drukuje 1.<br/>
**WINNING_SEQUENCE_EXIST <N>** odpowiada na pytanie czy istnieje dla któregokolwiek z graczy wygrywająca sekwencja posunięć mniejsza równa N.<br/>
**SOLVE_GAME_STATE** odpowiedź na pytanie dla którego z graczy dany planszy gry jest wygrywający. Możliwe są dwie odpowiedzi WHITE_HAS_WINING_STRATEGY albo BLACK_HAS_WINING_STRATEGY.<br/>
