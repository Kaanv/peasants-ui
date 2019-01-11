number = 0

function ai_turn(cards, tableCards, history)
    command = ""
    print "TEST AI FUNCTION!"
    print("NUMBER " .. number)
    number = number + 1
    numberOfCards = cards:numberOfCards()
    for i=0, numberOfCards - 1 do
        card = cards:at(i)
        print(card.value)
        print(card.color)
        print(card.selected)
    end

    if history:lengthOfHistory() == 0 then
        print("LENGTH " .. history:lengthOfHistory())
        command = "THROW"

        for i=0, numberOfCards - 1 do
            card = cards:at(i)
            if (card.value == 7) then
                command = command .. " " .. i
            end
            print(card.value)
            print(card.color)
            print(card.selected)
        end 
    else
        command = "PASS TURN" --THROW 0 1 2 3
    end
    return command
end

