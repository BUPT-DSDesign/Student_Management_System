import { defineStore } from 'pinia'
import { EventTable, AddEvent, DeleteEvent, SearchEvent } from '@/api/event'
import { ref } from 'vue'

export const useEventStore = defineStore('event', () => {
    const eventList = ref([])
    const searchList=ref([])
    const errMsg = ref('')
    const GetEventTable = async () => {
        try {
            const res = await EventTable()

            if (res.data.status_code == 0) {
                console.log(res.data.activities)
                eventList.value = res.data.activities
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }
    //添加活动
    const AddEventInfo = async (data) => {
        try {
            const res = await AddEvent(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
            else {
                errMsg.value = res.data.status_msg
            }
            return false
        } catch (err) {
            return false
        }
    }

    //删除活动
    const DeleteEventInfo = async (data) => {
        try {
            const res = await DeleteEvent(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                
                return true
            }
        } catch (err) {
            return false
        }
    }

    const SearchEventInfo = async (fromTime, endTime) => {
        try {
            const res = await SearchEvent(fromTime, endTime)
            console.log(res.data)
            if (res.data.status_code == 0) {
                searchList.value = res.data.activities
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        eventList,
        searchList,
        GetEventTable,
        AddEventInfo,
        DeleteEventInfo,
        SearchEventInfo,
        errMsg
    }
})
