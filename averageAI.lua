numberOfPlayers = 0
myPlayerId = 0

--ai_init is a function that gives you initial data. Don't modify its name

function ai_init(playersNumber, playerId)
    print "I AM AVERAGE AI!"
    numberOfPlayers = playersNumber
    myPlayerId = playerId
end

--ai_turn is a function used to calculate AI move. Don't modify its name

function ai_turn(cards, tableCards, history)
    command = ""

    if history:lengthOfHistory() == 0 then
        throw_starting_cards(cards)
    else
        cards_higher_than_on_table = find_cards_higher_than_given_value(cards, tableCards:numberOfCards(), tableCards:at(0).value)
        if all_players_passed(history) then
            index1 = find_smallest_card_except_index(cards)
            command = "THROW " .. index1
        elseif #cards_higher_than_on_table > 0 then
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

--indicate_peasant_level is a function that indicates peasant level after each round. Don't modify its name

function indicate_peasant_level(peasantLevel)

end

--give_cards_to_peasant is a function used when you are master to swap cards with your peasant. Don't modify its name

function give_cards_to_peasant(cards, peasantLevel)
    index1 = find_smallest_card_except_index(cards)
    index2 = find_smallest_card_except_index(cards, index1)
    index3 = find_smallest_card_except_index(cards, index1, index2)
    return index1 .. " " .. index2 .. " " .. index3
end

function find_smallest_card_except_index(cards, index, index2)
    smallest = cards:at(0)
    smallest_index = 0

    for i=1, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value < smallest.value and (index == nil or index ~= i or index2 ~= i)) then
            smallest = card
            smallest_index = i
        end
    end

    return smallest_index
end

function throw_starting_cards(cards, numberOfCards)
    command = "THROW"

    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (numberOfPlayers == 4 and card.value == 7 or
            numberOfPlayers == 5 and card.value == 5 or
            numberOfPlayers == 6 and card.value == 3) then
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

function all_players_passed(history)
    if history:lengthOfHistory() < numberOfPlayers then
        return false
    else
        for i=1, history:lengthOfHistory() do
            if history:at(history:lengthOfHistory() - i).playerId == myPlayerId then
                return true
            elseif history:at(history:lengthOfHistory() - i).action ~= "PASS TURN" then
                return false
            end
        end
        return false
    end
end


