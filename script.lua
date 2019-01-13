number = 0

--ai_turn is a function used to calculate AI move. Don't modify its name

function ai_turn(cards, tableCards, history)
    command = ""
    print "TEST AI FUNCTION!"
    print("NUMBER_OF_AI_RUNS " .. number)
    number = number + 1
    print_cards(cards, numberOfCards)

    if history:lengthOfHistory() == 0 then
        throw_starting_cards(cards)
    else
        command = "PASS TURN"
    end
    return command
end

function throw_starting_cards(cards, numberOfCards)
    command = "THROW"

    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value == 7) then
            command = command .. " " .. i
        end
    end 
end

function print_cards(cards)
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        print(card.value)
        print(card.color)
        print(card.selected)
    end
end

