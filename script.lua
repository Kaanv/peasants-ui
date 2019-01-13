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
        print("CARDS ON TABLE: ")
        print_cards(tableCards)
        cards_higher_than_on_table = find_cards_higher_than_given_value(cards, tableCards:numberOfCards(), tableCards:at(0).value)
        if #cards_higher_than_on_table > 0 then
            command = "THROW"
            for i=1, #cards_higher_than_on_table[1] do
                command = command .. " " .. cards_higher_than_on_table[1][i]
            end
        else
            command = "PASS TURN"
        end
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

function count_cards_of_given_value(cards, value)
    noOfCards = 0
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value == value) then
            noOfCards = noOfCards + 1
        end
    end
    return noOfCards
end

function get_indexes_of_cards_with_given_value(cards, value)
    indexes = {}
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value == value) then
            indexes[#indexes + 1] = i
            print "SOMETHING ADDED!"
        end
    end
    return indexes
end

function find_cards_higher_than_given_value(cards, numberOfCards, value)
    return_array = {}

    for i=0, cards:numberOfCards() - 1 do
        if cards:at(i).value > value then
            noOfCards = count_cards_of_given_value(cards, cards:at(i).value)
            if noOfCards == numberOfCards then
                return_array[#return_array + 1] = get_indexes_of_cards_with_given_value(cards, cards:at(i).value)
            end
        end
    end

    return return_array
end

