numberOfPlayers = 0
myPlayerId = 0
currentPeasantLevel = 0
cardsInGame = {}

--ai_init is a function that gives you initial data. Don't modify its name

function ai_isnit(playersNumber, playerId)
    print "I AM HARD AI!"
    numberOfPlayers = playersNumber
    myPlayerId = playerId
    set_cards_in_game()
end

function set_cards_in_game()
    if (numberOfPlayers == 4) then 
        cardsInGame = {0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4}
    elseif (numberOfPlayers == 5) then 
        cardsInGame = {0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
    else
        cardsInGame = {0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
    end
end

function get_number_of_highest_cards_in_hand(cards)
    numberOfHighestCards = 0
    currentHighestValue = get_current_highest_value()
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value == currentHighestValue) then
            numberOfHighestCards = numberOfHighestCards + 1
        end
    end
    return numberOfHighestCards
end

function get_current_highest_value()
    for i=0, #cardsInGame-1 do
        if (cardsInGame[#cardsInGame - i] > 0) then
            return (#cardsInGame - i);
        end
    end
end

function remove_cards_in_history_from_cards_in_game(history)
    set_cards_in_game()
    for i=0, history:lengthOfHistory()-1 do
        if (history:at(i).action ~= "PASS TURN") then
            for j=0, history:at(i).cards:numberOfCards()-1 do
                cardsInGame[history:at(i).cards:at(j).value] = cardsInGame[history:at(i).cards:at(j).value] - 1;
            end
        end
    end
end

function get_index_of_my_highest_card(cards)
    highestValue = cards:at(0).value
    highestIndex = 0
    for i=1, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value > highestValue) then
            highestValue = card.value
            highestIndex = i
        end
    end
    return highestIndex
end

function get_indexes_of_my_highest_cards(cards)
    highestValue = cards:at(get_index_of_my_highest_card(cards)).value
    highestIndexes = {}
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        if (card.value == highestValue) then
            highestIndexes[#highestIndexes + 1] = i
        end
    end
    return highestIndexes
end


--ai_turn is a function used to calculate AI move. Don't modify its name

function ai_turn(cards, tableCards, history)
    remove_cards_in_history_from_cards_in_game(history)
    highestCards = get_number_of_highest_cards_in_hand(cards)

    command = ""
    if history:lengthOfHistory() == 0 then
        throw_starting_cards(cards)
    else
        cards_higher_than_on_table = find_cards_higher_than_given_value(cards, tableCards:numberOfCards(), tableCards:at(0).value)
        if all_players_passed(history) then
            command = create_throw_highest_group(cards)
        elseif (#cards_higher_than_on_table > 0) then
            if (highestCards > 0 and tableCards:numberOfCards() == 1 and tableCards:at(0).value >= (get_current_highest_value() - 2)) then
                indexes = get_indexes_of_my_highest_cards(cards)
                command = "THROW"
                for i=1, tableCards:numberOfCards() do
                    command = command .. " " .. indexes[i]
                end
            else
                command = "THROW"
                index = choose_smallest_value_out_of_card_indexes(cards, cards_higher_than_on_table)
                indexes = cards_higher_than_on_table[index]
                for i=1, #indexes do
                    command = command .. " " .. indexes[i]
                end
            end
        else
            command = "PASS TURN"
        end
    end
    return command
end

function create_throw_highest_group(cards)
    index = find_smallest_card_except_index(cards)
    indexes = find_highest_group_of_cards_with_smallest_value(cards)
    command = "THROW"
    for i=1, #indexes do
        command = command .. " " .. indexes[i]
    end
    return command
end

function find_highest_group_of_cards_with_smallest_value(cards)
    cardsCount = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        cardsCount[card.value] = cardsCount[card.value] + 1
    end

    bestValue = 0
    bestCount = 0
    
    for i=3, #cardsCount do
        if (cardsCount[i] > bestCount) then
            bestCount = cardsCount[i]
            bestValue = i
        end
    end

    return get_indexes_of_cards_with_given_value(cards, bestValue)
end

--indicate_peasant_level is a function that indicates peasant level after each round. Don't modify its name

function indicate_peasant_level(peasantLevel)
    currentPeasantLevel = peasantLevel
    set_cards_in_game()
end

--give_cards_to_peasant is a function used when you are master to swap cards with your peasant. Don't modify its name

function give_cards_to_peasant(cards, peasantLevel)
    indexes = find_smallest_non_paired_cards(cards)

    index1 = find_smallest_card_except_index(cards)
    index2 = find_smallest_card_except_index(cards, index1)
    index3 = find_smallest_card_except_index(cards, index1, index2)

    if (#indexes >= 3) then
        index1 = indexes[1]
        index2 = indexes[2]
        index3 = indexes[3]
    elseif (#indexes == 2) then
        index2 = indexes[1]
        index3 = indexes[2]
    elseif (#indexes >= 1) then
        index3 = indexes[1]
    end

    return index1 .. " " .. index2 .. " " .. index3
end

function find_smallest_non_paired_cards(cards)
    cardsCount = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    for i=0, cards:numberOfCards() - 1 do
        card = cards:at(i)
        cardsCount[card.value] = cardsCount[card.value] + 1
    end

    values = {}
    for i=1, #cardsCount do
        if (cardsCount[i] == 1) then
            values[#values + 1] = i
        end
    end

    result = {}
    for i=1, #values do
        single_res = get_indexes_of_cards_with_given_value(cards, values[i])
        result[#result + 1] = single_res[1]
    end

    return result;
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

function choose_smallest_value_out_of_card_indexes(cards, cards_indexes)
    smallest_index = 1
    smallest_value = cards:at(cards_indexes[smallest_index][1]).value;

    for i=1, #cards_indexes do
        if cards:at(cards_indexes[i][1]).value < smallest_value then
            smallest_index = i
            smallest_value = cards:at(cards_indexes[i][1]).value
        end
    end

    return smallest_index
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

